#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int main(int argc, char **argv) {
    int c = 0;
    int iflag = 0, oflag = 0, cflag = 0, pflag = 0, rflag = 0;
    FILE *input_file;

    while ((c = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        switch(c) {
            case 'i':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing Argument_i");
                    return MISSING_ARGUMENT;
                }
                if (iflag) {
                    printf("Duplicate Arguement_i");
                    return DUPLICATE_ARGUMENT;
                }

                iflag = 1;
                input_file = fopen(optarg, "r");

                break;
            case 'o':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing Argument_i");
                    return MISSING_ARGUMENT;
                }
                if (oflag) {
                    printf("Duplicate Arguement_o");
                    return DUPLICATE_ARGUMENT;
                }

                oflag = 1;
                break;
            case 'c':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing Argument_i");
                    return MISSING_ARGUMENT;
                }
                if (cflag) {
                    printf("Duplicate Arguement_c");
                    return DUPLICATE_ARGUMENT;
                }

                cflag = 1;
                break;
            case 'p':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing Argument_i");
                    return MISSING_ARGUMENT;
                }
                if (pflag) {
                    printf("Duplicate Arguement_p");
                    return DUPLICATE_ARGUMENT;
                }

                pflag = 1;
                break;
            case 'r':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing Argument_i");
                    return MISSING_ARGUMENT;
                }
                if (rflag) {
                    printf("Duplicate Arguement_r");
                    return DUPLICATE_ARGUMENT;
                }

                rflag = 1;
                break;
            case '?':
                printf("Unrecognized Arguemnt");
                return UNRECOGNIZED_ARGUMENT;
        }
    }

    if (!oflag || !iflag) {
        printf("missing argument");
        return MISSING_ARGUMENT;
    }

    if (input_file == NULL) {
        return INPUT_FILE_MISSING;
    }

    return 0;
}
