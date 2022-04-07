#include "findc.h"

int main(int argc, char** argv) {
    char* filename = '\0';
    char searchChar = 'c';
    int searchMode;
    FILE *input = stdin;

    int opt, count;

    while(( opt= getopt(argc, argv, "hf:c:")) != -1)
    {
        switch (opt) {
            case 'f':
                // filename = optarg;
                searchMode = 1;
                break;
            case 'c':
                // searchChar = optarg[0];
                break;
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            default:
                searchMode = 0;
        }
    }

    if (searchMode == 1){
        count =  numCharInFile(filename, searchChar);
        printCount(searchChar, count);
        // return 0;
    }

    if(searchMode == 0){
        count =  numCharInput(input, searchChar);
        printCount(searchChar, count);
    }


    return 0;

}
