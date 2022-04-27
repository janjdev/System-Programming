#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>



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

        // If the element is present at the middle
        // itself
        if (arr[mid] == x)
            return mid;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid + 1, r, x);
    }

    // We reach here when element is not
    // present in array
    return -1;
}

int linearSearch(int arr[], int x, int len)
{
    int arrLen = len;
    int isElementPresent = 0;

    for (int i = 0; i < arrLen; i++) {
        if (arr[i] == x) {
            isElementPresent = 1;
            break;
        }
    }

    return isElementPresent;
}

int main(int argc, char* argv[])
{
    int opt, search, result, status, *numbers = 0, len = argc-2, count=0, pairCount=0;
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

    // if(argc < 3)
    // {
    //     printf("argc is %d. Not enough arguments provided.\n", argc );
    //     printHelp();
    //     exit(EXIT_FAILURE);
    // }
    //Fork child process
    child_id = fork();

    if (child_id == 0)
    {
        numbers = malloc(sizeof(int)*len);
        if(numbers == 0)
        {
            printf("Failed to allocate memory.\n");
            return 1;
        }
         //open file (or create one) for writing
        char* filename = argv[1];
        file = fopen(filename, "a+");
        if(file == NULL)
        {
            fprintf(stderr, "\n%s: cannot open or create %s.\n", argv[0], argv[1]);
            printHelp();
            exit(EXIT_FAILURE);
        }

        for(int i = 2; i < argc; i++)
        {
            //write each number to the file
            if(i == 2){
                fprintf(file, "Child: %d: ", getpid());
            }
            fprintf(file, "%d ", atoi(argv[i]));

            //Popolaute the dynamic array
            numbers[count] = atoi(argv[i]);
            count++;
        }
        fprintf(file, "\n");

        //create an array of pairs
        int *pairs =  malloc(sizeof(int)*count);

        //sort the array for faster searching
        qsort (numbers, count, sizeof(int), compare);

        //Find the pairs the sum to 19
        for(int i=0; i < count; i++ ){
            search = ((numbers[i] -19) * -1);
            result = binarySearch(numbers, 0, len - 1, search);

            if(result > -1)
            {
                if(pairCount == 0)
                {
                    fprintf(file, "Child: %d Pair:" , getpid());
                }
                if(linearSearch(pairs, search, count) != 1) //check that we haven't already gotten this pair
                {
                    pairs[i] = numbers[i];
                    pairCount++;

                    fprintf(file, " %d %d", numbers[i], numbers[result]);
                }
            }
        }
        fprintf(file, "\n");
        fclose(file);
        free(pairs);

        if(pairCount > 0){
             _exit(EXIT_SUCCESS);
        }else
        {
             _exit(EXIT_FAILURE);
        }

    }
    if (child_id < 0)
    {
        _exit(EXIT_FAILURE);
    }

     //Make parent process wait
    parent_id = wait(&status);

    //open file (or create one) for writing
        char* filename = argv[1];
        file = fopen(filename, "a+");
        if(file == NULL)
        {
            fprintf(stderr, "%s: cannot open or create %s.\n", argv[0], argv[1]);
            printHelp();
            exit(EXIT_FAILURE);
        }
    if(status == 0){
       fprintf(file, "Parent id=%d, EXIT_SUCCESS\n", getpid());
    }
    else
    {
         fprintf(file, "Parent id=%d, EXIT_FAILURE\n", getpid());
    }


    fclose(file);
    free(numbers);



    return 0;
}

