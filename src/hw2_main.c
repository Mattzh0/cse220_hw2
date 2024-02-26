#include "hw2.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h> 

int colortable_exists(int *table, int table_size, int r, int g, int b);

int main(int argc, char **argv) {
    int c = 0;
    int iflag = 0, oflag = 0, cflag = 0, pflag = 0, rflag = 0;
    FILE *input_file;
    char input_path[256] = "";
    FILE *output_file;
    char output_path[256] = "";
    FILE *font_file;
    int c_param_len = 0, p_param_len = 0, r_param_len = 0;

    int copy_row, copy_col, copy_width, copy_height;
    int paste_row, paste_col;

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
                strcpy(input_path,optarg);
                input_file = fopen(input_path, "r");
                break;
            case 'o':
                if (optind + 1 == argc || optarg == NULL || (optarg && optarg[0] == '-')) {
                    return MISSING_ARGUMENT;
                }
                if (oflag) {
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
                    switch (c_param_len) {
                        case 0:
                            copy_row = atoi(token_c);
                            break;
                        case 1:
                            copy_col = atoi(token_c);
                            break;
                        case 2:
                            copy_width = atoi(token_c);
                            break;
                        case 3:
                            copy_height = atoi(token_c);
                            break;
                    }
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
                    switch (p_param_len) {
                        case 0:
                            paste_row = atoi(token_p);
                            break;
                        case 1:
                            paste_col = atoi(token_p);
                            break;
                    }
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

    //flags to indicate if input is ppm or sbu
    int input_ppm_flag = 0;
    int input_sbu_flag = 0;
    char *input_extension = strrchr(input_path, '.');
    if (strcmp(input_extension, ".ppm") == 0) {
        input_ppm_flag = 1;
    } else if (strcmp(input_extension, ".sbu") == 0) {
        input_sbu_flag = 1;
    }

    //flags to indicate if output is ppm or sbu
    int output_ppm_flag = 0;
    int output_sbu_flag = 0;
    char *output_extension = strrchr(output_path, '.');
    if (strcmp(output_extension, ".ppm") == 0) {
        output_ppm_flag = 1;
    } else if (strcmp(output_extension, ".sbu") == 0) {
        output_sbu_flag = 1;
    }

    if (input_ppm_flag) {
        char p3[3];
        int width, height, color_num;
        fscanf(input_file, "%s", p3);
        fscanf(input_file, "%d %d", &width, &height);
        fscanf(input_file, "%d", &color_num);

        //store the ppm input pixel data in a 1d array
        int *pixels = malloc(width * height * 3 * sizeof(int));
        int pixels_size = width*height*3;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int red,green,blue;
                fscanf(input_file, "%d %d %d", &red, &green, &blue);
                pixels[(i * width + j) * 3] = red;
                pixels[(i * width + j) * 3 + 1] = green;
                pixels[(i * width + j) * 3 + 2] = blue;
            }
        }

        //input = ppm, output = ppm
        if (output_ppm_flag) {
            int *output_pixels = malloc(width * height * 3 * sizeof(int));
            memcpy(output_pixels, pixels, width * height * 3 * sizeof(int));

            //if there is something to copy and paste
            if (cflag && pflag) {
                for (int i = 0; i < copy_height; i++) {
                    for (int j = 0; j < copy_width; j++) {
                        int src_row = copy_row + i;
                        int src_col = copy_col + j;
                        int dst_row = paste_row + i;
                        int dst_col = paste_col + j;
                        
                        if (src_row >= 0 && src_row < height && src_col >= 0 && src_col < width && dst_row >= 0 && dst_row < height && dst_col >= 0 && dst_col < width) {
                            int src = (src_row * width + src_col) * 3;
                            int dst = (dst_row * width + dst_col) * 3;

                            output_pixels[dst] = pixels[src];
                            output_pixels[dst+1] = pixels[src+1];
                            output_pixels[dst+2] = pixels[src+2];
                        }
                    }
                }
            }

            // TODO: account for case when there is -r argument

            fprintf(output_file, "%s\n%d %d\n%d\n", p3, width, height, color_num);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    fprintf(output_file, "%d %d %d ", output_pixels[(i * width + j) * 3], output_pixels[(i * width + j) * 3 + 1], output_pixels[(i * width + j) * 3 + 2]);
                }
                fprintf(output_file, "\n");
            }
            fclose(output_file);
            free(output_pixels);
        }
        //input = ppm, output = sbu
        else if (output_sbu_flag) {
            int *color_table = malloc(width * height * 3 * sizeof(int));
            int color_table_size = 0;
            for (int i = 0; i < pixels_size; i += 3) {
                if (!colortable_exists(color_table, color_table_size, pixels[i],pixels[i+1],pixels[i+2])) {
                    color_table[color_table_size] = pixels[i];
                    color_table[color_table_size+1] = pixels[i+1];
                    color_table[color_table_size+2] = pixels[i+2];
                    color_table_size += 3;
                }
            }






            free(color_table);
        }
        
        free(pixels);
    }
    else if (input_sbu_flag) {
        char sbu[4];
        int width, height;
        fscanf(input_file, "%s", sbu);
        fscanf(input_file, "%d %d", &width, &height);
    }

    return 0;
}

int colortable_exists(int *table, int table_size, int r, int g, int b) {
    for (int i = 0; i < table_size; i += 3) {
        if (table[i] == r && table[i+1] == g && table[i+2] == b) {
            return 1;
        }
    }
    return 0;
}

