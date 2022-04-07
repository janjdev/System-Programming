#ifndef FINDC_H
#define FINDC_H


#include <stdio.h>
#include <stdlib.h>
// #include <getopt.h>

//Get the count of the desired character
int numCharInFile(char* filename, char searchChar)
{
    FILE* fp = fopen(filename, "r");
    if (!fp || filename[0] == '\0')
    {
        fprintf(stderr, "The file %s cannot open. Error Occurred while Opening the File!\n", filename);
        return(-1);
    }

    int count = 0;
    int currentChar;
    while ((currentChar = fgetc(fp)) != EOF)
    {
        count+=(currentChar == searchChar);
    }
    fclose(fp);
    return  count;
}


int numCharInput(FILE *input, char searchChar)
{

    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) < 0) {
         printf("The file is empty or cannot be open.\n");
        return(-1);
    }

        rewind(stdin);
        int count = 0;
        int currentChar;
        while ((currentChar = fgetc(input)) != EOF)
        {
            count+=(currentChar == searchChar);
        }
        return  count;
}

void printHelp() {
    printf("Usage: ./findC [OPTION] \n\n");
    printf("Options: \n");
    printf("    -f filename, specify a file to search. (stdin by default)\n");
    printf("    -c X,        specify the character to search for (c by default)\n");
    printf("    -h           print this help\n");
}

void printCount(char searchChar, int count)
{
    printf("Number of %c's found: %d\n", searchChar, count);
}

#endif //FINDC_H
