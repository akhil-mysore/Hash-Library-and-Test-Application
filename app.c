

/* Application */
/* TODO: The code is not even compilable .. we have to also change dll functions to ahsh */

/* Test program */
int main(void)
{
    unsigned short choice;
    int data;
    hasht hptr;

    hash_init(1000, &head); // Initialize the doubly linked list

    while(1)
    {
        printf("1. Add an entry into DLL\n"
              "2. Delete an entry from DLL\n"
              "3. Dump Linked List Items\n"
              "4. Exit");

        scanf("%d", &choice);

        switch(choice)
        {
        case 1:
            printf("Enter the data (integer) to be inserted");
            scanf("%d", &data);
            dll_insert(&head, data);
            break;

        case 2:
            printf("Enter the data (integer) to be deleted");
            scanf("%d", &data);
            dll_delete(&head, data);
            break;
            
        case 3:
            printf("Dumping all the nodes...");
            dll_dump(&head);
            break;

        case 4:
            // Free any memory alocated and gracefully exit
            exit(0); 

        default:
            printf("Naughty !! Enter 1 or 2 or 3 or 4 .. you have no other option\n");
            break;
        }
    }
    return 0;
}

