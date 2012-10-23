
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_lib.h"

//#define stdin 0

int main(void)
{
    int choice;
    int data, key, bytes_read;
    size_t buf_len;
    hasht hptr;
    char *buf = NULL;
    hash_ret_e hash_ret;

    int c;


    hptr = hash_init(32, NULL, NULL);

    while(1)
    {
        printf("1. Add an entry\n"
               "2. Delete an entry\n"
               "3. Dump Items\n"
               "4. Exit\n\n");

        printf("Your Input: ");

        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            printf("Enter key (interger) and data (name) to be inserted\n");
            printf("Your Input: ");
            scanf("%d", &key);
            buf_len = 10;
            buf = (char *) malloc (buf_len);
            if(!buf)
                exit(0);

            // Flush stdin of the last enter while reading the key
            while((c = getchar()) != '\n' && c != EOF);

            printf("\nEnter data (line): ");
            
            bytes_read = getline(&buf, &buf_len, stdin);
            if(-1 == bytes_read)
            {
                printf("Read Error, try again\n");
            }
            else
            {
                printf("byes_read = %d. You typed: %s\n", bytes_read, buf);
                hash_ret = hash_insert(hptr, &key, buf);
                if(hash_ret == HASH_OK)
                    printf("Inserted Sucessfully\n");
                else
                    printf("Hash Insert Failes %u\n", hash_ret);
            }


            break;

        case 2:
            printf("Enter the key (integer) to be deleted");
            scanf("%d", &key);
            hash_remove(hptr, &key);
            break;
            
        case 3:
            printf("Dumping all the nodes...");
            //
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

