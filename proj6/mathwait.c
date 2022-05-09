#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//============================================================================//
// Name        : mathwait.c                                                   //
// Author      : January Johnson                                              //
// Description : Create a program that takes in an array, allocates shared    //
//               memory for two integers and creates a child process that     //
//               finds two numbers that add to 19 from the array. If a pair   //
//               is found, put the pair into the shared memory, else put      //
//               -1,-1. Out put the found pair in the parent process.         //
//============================================================================//


#define SHMKEY	389809             /* Parent and child agree on common key.*/
#define BUFF_SZ	sizeof ( int )


void printHelp() {
    printf("Usage: ./mathwait [OPTION] \n\n");
    printf("Options: \n");
    printf(" You must specify a file to write to and a series of numbers to be written to the file.\n");
    printf(" Example: ./mathwait File.txt 32 9 10 -13\n");
    printf(" -h  Print help\n");

}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int binarySearch(int arr[], int l, int r, int x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;

        if (arr[mid] == x)
            return mid;

        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);

        return binarySearch(arr, mid + 1, r, x);
    }

    return -1;
}

int main(int argc, char* argv[])
{
    int opt, status, count=0;
    pid_t parent_id, child_id;
    FILE * file;

    while(( opt= getopt(argc, argv, "h")) != -1)
    {
        switch (opt) {
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            default:
                printHelp();
                exit(EXIT_FAILURE);
        }
    }

    //check for file
    if(strspn(argv[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./") != strlen(argv[1]))
    {
        printf("\n%s: Cannot open or create file %s.\n", argv[0], argv[1]);
        printHelp();
        exit(EXIT_FAILURE);
    }
    else
    {
        char* filename = argv[1];
        file = fopen(filename, "r+");
    }

    if(!file)
    {
        printf("\n%s: Cannot open file %s. Please, check that the file exist.\n", argv[0], argv[1]);
        printHelp();
        exit(EXIT_FAILURE);
    }

    //Allocate shared memory
    int shmid = shmget(SHMKEY, 2*BUFF_SZ, 0666 | IPC_CREAT);
    if(shmid == -1){
        printf("Failed to allocate memory.\n");
        return shmid;
    }
    //attach to the shared memory
    char * paddr = (char * )(shmat(shmid, 0, 0));
    int * pair = (int *)(paddr);

    pair[0] = -2;
    pair[1] = -2;


    //Fork child process
    child_id = fork();

    if (child_id == 0)
    {
        int len = argc-2;

        //create the dynamic array
        int *numbers = malloc(sizeof(int)*len);

        if(numbers == 0)
        {
            fprintf(file, "Failed to allocate memory.\n");
            return 1;
        }
        for(int i = 2; i < argc; i++)
        {
            //Popolaute the dynamic array
            numbers[count] = atoi(argv[i]);
            count++;
        }

        //sort the array for faster searching
        qsort (numbers, count, sizeof(int), compare);

        //attach to the shared memory
        int * match = ( int * )( shmat ( shmid, 0, 0 ) );

        if(match[0] != -2 || match[1] != -2)
        {
            fprintf(stderr, "Error allocating memory. -2 not found in shared memory.\n");
            _exit(EXIT_FAILURE);
        }
        //Find the pairs the sum to 19
        for(int i=0; i < count; i++ ){
            int search = ((numbers[i] -19) * -1);
            int result = binarySearch(numbers, 0, len - 1, search);

            if(result > -1)
            {
                match[0]=numbers[i];
                match[1] = numbers[result];
                shmdt(match);
                free(numbers);
                _exit(EXIT_SUCCESS);
            }
        }
        match[0]=-1;
        match[1] =-1;
        shmdt(match);
        free(numbers);
        _exit(EXIT_SUCCESS);
    }
    if (child_id < 0)
    {
        _exit(EXIT_FAILURE);
    }

     //Make parent process wait
    parent_id = wait(&status);

    if(status == 0){
        if(pair[0] != pair[1])
        {
            printf("Pair found by child: %d %d\n", pair[0], pair[1]);
        }
    }
    shmdt(pair);
    shmctl(shmid,IPC_RMID,NULL);
    fclose(file);
    return 0;
}

