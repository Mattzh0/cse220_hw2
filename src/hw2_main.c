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
    FILE *output_file;
    FILE *font_file;
    int c_param_len = 0, p_param_len = 0, r_param_len = 0;

    while ((c = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        switch(c) {
            case 'i':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    return MISSING_ARGUMENT;
                }
                if (iflag) {
                    return DUPLICATE_ARGUMENT;
                }
                iflag = 1;
                input_file = fopen(optarg, "r");
                break;
            case 'o':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    return MISSING_ARGUMENT;
                }
                if (oflag) {
                    return DUPLICATE_ARGUMENT;
                }
                oflag = 1;
                output_file = fopen(optarg, "w");
                break;
            case 'c':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    return MISSING_ARGUMENT;
                }
                if (cflag) {
                    return DUPLICATE_ARGUMENT;
                }
                cflag = 1;
                
                char* token_c = strtok(optarg, ",");
                while (token_c) {
                    c_param_len += 1;
                    token_c = strtok(NULL, ",");
                }

                break;
            case 'p':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    return MISSING_ARGUMENT;
                }
                if (pflag) {
                    return DUPLICATE_ARGUMENT;
                }
                pflag = 1;

                char* token_p = strtok(optarg, ",");
                while (token_p) {
                    p_param_len += 1;
                    token_p = strtok(NULL, ",");
                }

                break;
            case 'r':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    return MISSING_ARGUMENT;
                }
                if (rflag) {
                    return DUPLICATE_ARGUMENT;
                }
                rflag = 1;

                char* token_r = strtok(optarg, ",");
                while (token_r) {
                    r_param_len += 1;
                    token_r = strtok(NULL, ",");

                    if (r_param_len == 2) {
                        font_file = fopen(token_r, "r");
                    }
                }

                break;
            case '?':
                return UNRECOGNIZED_ARGUMENT;
        }
    }

    if (!oflag || !iflag) {
        return MISSING_ARGUMENT;
    }
    if (input_file == NULL) {
        return INPUT_FILE_MISSING;
    }
    if (output_file == NULL) {
        return OUTPUT_FILE_UNWRITABLE;
    }
    if (pflag && !cflag) {
        return C_ARGUMENT_MISSING;
    }
    if (cflag && c_param_len != 4) {
        return C_ARGUMENT_INVALID;
    }
    if (pflag && p_param_len != 2) {
        return P_ARGUMENT_INVALID;
    }
    if (rflag && (font_file == NULL || r_param_len != 5)) {
        return R_ARGUMENT_INVALID;
    }
    return 0;
}
