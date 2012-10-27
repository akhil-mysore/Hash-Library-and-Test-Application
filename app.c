
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_lib.h"

//#define stdin 0

typedef struct info_s
{
    int key;
    char *data;
} info_t;

static void hash_dump_function(p_key arg1, p_data arg2, bucket_t index)
{
    int key = *((int *) arg1);
    info_t *info = (info_t *) arg2;

    printf("For key: %u (index: %u) --> %s\n", key, index, info->data);
}

static bucket_t hash_function(p_key arg1, bucket_t table_size)
{
  int key = *((int *) arg1);
  return (bucket_t)(key % table_size);
}

static hash_ret_e compare_function(p_key arg1, p_key arg2)
{
  int compare_this = *((int *) arg1);
  int with_this = *((int *) arg2);

  if(compare_this == with_this)
    return HASH_OK;
  else
    return HASH_NOTOK;
}


int main(void)
{
    int choice;
    int bytes_read;
    size_t buf_len;
    hasht hptr;
    hash_ret_e hash_ret;
    info_t *info;
    int key;

    int c;

    hptr = hash_init(2, hash_function, compare_function, hash_dump_function);

    while(1)
    {
        printf("\n\t1. Add an entry\n"
               "\t2. Delete an entry\n"
               "\t3. Dump Items\n"
               "\t4. Exit\n\n");

        printf("Your Input: ");

        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            info = (info_t *) malloc (sizeof(info_t));
            if(!info)
                exit(0);

            printf("Enter key (interger) and data (name) to be inserted\n");
            printf("Your Input: ");
            scanf("%d", &info->key);
            buf_len = 10;
            info->data = (char *) malloc (buf_len);
            if(!info->data)
                exit(0);

            // Flush stdin of the last enter while reading the key
            while((c = getchar()) != '\n' && c != EOF);

            printf("\nEnter data (line): ");
            
            bytes_read = getline(&info->data, &buf_len, stdin);
            if(-1 == bytes_read)
            {
                printf("Read Error, try again\n");
            }
            else
            {
                info->data[bytes_read] = '\0';
                printf("byes_read = %d. You typed: %s\n", bytes_read, info->data);
                hash_ret = hash_insert(hptr, &info->key, info);
                if(hash_ret == HASH_OK)
                    printf("Inserted Sucessfully\n");
                else
                    printf("Hash Insert Failed. ret_val = %u\n", hash_ret);
            }


            break;

        case 2:
            printf("Enter the key (integer) to be deleted");
            scanf("%d", &key);
            hash_remove(hptr, &key);
            break;
            
        case 3:
            printf("Dumping all the nodes...\n\n");
            hash_dump(hptr);
            break;

        case 4:
            // Free any memory alocated and gracefully exit
            exit(0); 

        default:
            printf("Enter 1 or 2 or 3 or 4 .. you have no other option\n");
            break;
        }
    }
    return 0;
}

