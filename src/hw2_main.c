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
    char input_path[256] = "";
    FILE *output_file;
    char output_path[256] = "";
    FILE *font_file;
    int c_param_len = 0, p_param_len = 0, r_param_len = 0;

    while ((c = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        switch(c) {
            case 'i':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing argument i");
                    return MISSING_ARGUMENT;
                }
                if (iflag) {
                    printf("Duplicate argument i");
                    return DUPLICATE_ARGUMENT;
                }
                iflag = 1;
                strcpy(input_path,optarg);
                input_file = fopen(input_path, "r");
                break;
            case 'o':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    printf("Missing argument o");
                    return MISSING_ARGUMENT;
                }
                if (oflag) {
                    printf("Duplicate argument o");
                    return DUPLICATE_ARGUMENT;
                }
                oflag = 1;
                strcpy(output_path,optarg);
                output_file = fopen(output_path, "w");
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
        printf("input file missing");
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

    int input_ppm_flag = 0;
    int input_sbu_flag = 0;
    char *input_extension = strrchr(input_path, '.');
    if (strcmp(input_extension, ".ppm") == 0) {
        input_ppm_flag = 1;
    } else if (strcmp(input_extension, ".sbu") == 0) {
        input_sbu_flag = 1;
    }

    if (input_ppm_flag) {
        char p3[3];
        int width, height, color_num;
        fscanf(input_file, "%s", p3);
        fscanf(input_file, "%d %d", &width, &height);
        fscanf(input_file, "%d", &color_num);

        int *pixels = malloc(width * height * 3 * sizeof(int));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int red,green,blue;
                fscanf(input_file, "%d %d %d", &red, &green, &blue);
                pixels[(i * width + j) * 3] = red;
                pixels[(i * width + j) * 3 + 1] = green;
                pixels[(i * width + j) * 3 + 2] = blue;
            }
        }

        free(pixels);
    }

    if (input_sbu_flag) {
        char sbu[4];
        int width, height;
        fscanf(input_file, "%s", sbu);
        fscanf(input_file, "%d %d", &width, &height);

    }

    return 0;
}

