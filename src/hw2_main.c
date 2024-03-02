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
void run_length_encoding(int *non_run_encoding);

int main(int argc, char **argv) {
    extern int optind, opterr, optopt;
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
    char *font_msg;
    int font_size, font_row, font_col;

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
                    switch (r_param_len) {
                        case 0:
                            font_msg = token_r;
                            break;
                        case 1:
                            font_file = fopen(token_r, "r");
                            break;
                        case 2:
                            font_size = atoi(token_r);
                            break;
                        case 3:
                            font_row = atoi(token_r);
                            break;
                        case 4:
                            font_col = atoi(token_r);
                                break;
                    }
                    r_param_len += 1;
                    token_r = strtok(NULL, ",");
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

    int input_ppm_flag = 0;
    int input_sbu_flag = 0;
    char *input_extension = strrchr(input_path, '.');
    if (strcmp(input_extension, ".ppm") == 0) {
        input_ppm_flag = 1;
    } else if (strcmp(input_extension, ".sbu") == 0) {
        input_sbu_flag = 1;
    }

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

        int *output_pixels = malloc(width * height * 3 * sizeof(int));
        memcpy(output_pixels, pixels, width * height * 3 * sizeof(int));

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

        if (rflag) {
            int font_txt_len = 0;
            int font_txt_rows = 0;
            int font_txt_cols= 0;
            char ch;

            while ((ch = fgetc(font_file)) != EOF) {
                if (ch == '\n') {
                    font_txt_rows++;
                } else {
                    font_txt_len++;
                }
            }

            font_txt_cols = font_txt_len/font_txt_rows;
            fseek(font_file, 0, SEEK_SET);

            char font_data[font_txt_rows][font_txt_cols];

            for (int row = 0; row < font_txt_rows; row++) {
                for (int col = 0; col < font_txt_cols; col++) {
                    ch = fgetc(font_file);
                    if (ch == ' ') {
                        font_data[row][col] = '.';
                    } 
                    else if (ch == '*') {
                        font_data[row][col] = ch;
                    }
                }
                fgetc(font_file);
            }

            int starting_indices[26];
            for (int i = 0; i < 26; i++) {
                starting_indices[i] = 0;
            }
            int current_index = 1;

            for (int col = 1; col < font_txt_cols; col++) {
                int whitespace_col = 0;
                for (int row = 0; row < font_txt_rows; row++) {
                    if (font_data[row][col] == '.') {
                        whitespace_col = 1;
                    }
                    else if (font_data[row][col] == '*'){
                        whitespace_col = 0;
                        break;
                    }
                }
                if (whitespace_col == 1) {
                    if (current_index >= 0 && current_index < 26) {
                        starting_indices[current_index] = col + 1;
                    }
                    current_index++;
                }
            }

            for (int i = 0; font_msg[i] != '\0'; i++) {
                char cur_char = toupper(font_msg[i]);
                if (cur_char != ' ') {
                    int code = cur_char - 'A';
                    int start_col = starting_indices[code];
                    int end_col;
                    if (code == 25) {
                        end_col = font_txt_cols - 1;
                    }
                    else {
                        end_col = starting_indices[(code+1)] - 1;
                    }


                    int *char_box_data = malloc((font_txt_rows) * (end_col - start_col) * 3 * sizeof(int));
                    int box_index = 0;
                    for (int row = 0; row < font_txt_rows; row++) {
                        for (int col = start_col; col < end_col; col++) {
                            if (font_data[row][col] == '*') {
                                char_box_data[box_index] = 255;
                                char_box_data[box_index+1] = 255;
                                char_box_data[box_index+2] = 255;
                                box_index += 3;
                            }
                            else {
                                char_box_data[box_index] = 0;
                                char_box_data[box_index+1] = 0;
                                char_box_data[box_index+2] = 0;
                                box_index += 3;
                            }
                        }
                    }

                    int cols = 0;
                    int start_position = (font_row * width + font_col) * 3;
                    if ((font_row + font_txt_rows - 1 <= height) && (font_col + (end_col - start_col) <= width)) {
                        for (int row = 0; row < font_txt_rows; row++) {
                        cols = (end_col - start_col);
                        for (int col = 0; col < cols; col++) {
                            int r = char_box_data[(row * cols + col) * 3];
                            int g = char_box_data[(row * cols + col) * 3 + 1];
                            int b = char_box_data[(row * cols + col) * 3 + 2];

                            if (r == 255 && g == 255 && b == 255 && (font_row + row) < height && (font_col + col) < width) {
                                output_pixels[start_position + (row * width * 3) + (col * 3)] = r;
                                output_pixels[start_position + (row * width * 3) + (col * 3) + 1] = g;
                                output_pixels[start_position + (row * width * 3) + (col * 3) + 2] = b;
                            }
                        }
                    } 
                    font_col += (cols + 1);
                    }

                    free(char_box_data);
                }
                else {
                    font_col += 5;
                }
                
            }
            // ./a.out -i "desert.ppm" -o "output.ppm" -r "seawolfseawolfseawolfseawolfseawolfseawolf","font1.txt",1,100,150
            // ./a.out -i "desert.ppm" -o "output.ppm" -r "hi","font1.txt",1,100,150
        }

        if (output_ppm_flag) {
            fprintf(output_file, "%s\n%d %d\n%d\n", p3, width, height, color_num);
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    fprintf(output_file, "%d %d %d ", output_pixels[(i * width + j) * 3], output_pixels[(i * width + j) * 3 + 1], output_pixels[(i * width + j) * 3 + 2]);
                }
                fprintf(output_file, "\n");
            }
        }
        else if (output_sbu_flag) {
            int *color_table = malloc(width * height * 3 * sizeof(int));
            int color_table_size = 0;
            int *encoding = malloc((pixels_size/3) * sizeof(int));

            for (int i = 0; i < pixels_size; i += 3) {
                int encode_index = colortable_exists(color_table, color_table_size, output_pixels[i],output_pixels[i+1],output_pixels[i+2]);
                if (encode_index == -1) {
                    color_table[color_table_size] = output_pixels[i];
                    color_table[color_table_size+1] = output_pixels[i+1];
                    color_table[color_table_size+2] = output_pixels[i+2];
                    encoding[i/3] = color_table_size/3;
                    color_table_size += 3;
                }
                else {
                    encoding[i/3] = encode_index;
                }
            }

            fprintf(output_file, "SBU\n");
            fprintf(output_file, "%d %d\n", width, height);
            fprintf(output_file, "%d\n", color_table_size/3);

            for (int i = 0; i < color_table_size; i++) {
                fprintf(output_file, "%d ", color_table[i]);
            }
            fprintf(output_file, "\n");

            for (int i = 0; i < pixels_size/3; i++) {
                int counter = 1;
                while ((i + counter) < (pixels_size/3) && (encoding[i] == encoding[i + counter])) {
                    counter++;
                }
                if (counter > 1) {
                    fprintf(output_file, "*%d %d ", counter, encoding[i]);
                }
                else {
                    fprintf(output_file, "%d ", encoding[i]);
                }
                i += counter - 1;
            }
            fclose(output_file);
            free(color_table);
            free(encoding);
        }
        free(output_pixels);
        free(pixels);
        fclose(input_file);
    }
    else if (input_sbu_flag) {
        char sbu[4];
        int width, height;
        int color_table_num_entries;
        fscanf(input_file, "%s", sbu);
        fscanf(input_file, "%d %d\n", &width, &height);
        fscanf(input_file, "%d\n", &color_table_num_entries);

        int color_table_size = color_table_num_entries*3;

        int *color_table = malloc((color_table_size) * sizeof(int));
        for (int i = 0; i < color_table_size; i++) {
            fscanf(input_file, "%d", &color_table[i]);
        }

        int *encoding = malloc(width * height * sizeof(int));

        int ind = 0;
        int value, count;
        while (ind < (width * height)) {
            if (fscanf(input_file, "%d", &value)) {
                encoding[ind++] = value;
            }
            else if (fscanf(input_file, "*%d %d", &count, &value)) {
                while (count > 0) {
                    encoding[ind++] = value;
                    count--;
                }
            }
        }

        int *encoding_copy = malloc(width * height * sizeof(int));
        memcpy(encoding_copy, encoding, width * height * sizeof(int));
        int *final_encoding = malloc(width * height * sizeof(int));
        memcpy(final_encoding, encoding, width * height * sizeof(int));

        if (cflag && pflag) {
            for (int i = 0; i < copy_height; i++) {
                for (int j = 0; j < copy_width; j++) {
                    int src_row = copy_row + i;
                    int src_col = copy_col + j;
                    int dst_row = paste_row + i;
                    int dst_col = paste_col + j;

                    if (src_row >= 0 && src_row < height && src_col >= 0 && src_col < width && dst_row >= 0 && dst_row < height && dst_col >= 0 && dst_col < width) {
                        int src = (src_row * width + src_col);
                        int dst = (dst_row * width + dst_col);
                        encoding_copy[dst] = encoding[src];
                    }
                }
            }

            int new_color_table_size = 0;
            int *new_color_table = malloc(width * height * 3 * sizeof(int));
            free(final_encoding);
            final_encoding = malloc(width * height * sizeof(int));
            for (int i = 0; i < width * height; i++) {
                int color_index = encoding_copy[i];
                int r = color_table[color_index * 3];
                int g = color_table[(color_index * 3) + 1];
                int b = color_table[(color_index * 3) + 2];

                int encode_index = colortable_exists(new_color_table, new_color_table_size, r, g, b);
                if (encode_index == -1) {
                    new_color_table[new_color_table_size] = r;
                    new_color_table[(new_color_table_size) + 1] = g;
                    new_color_table[(new_color_table_size) + 2] = b;
                    final_encoding[i] = new_color_table_size/3;
                    new_color_table_size += 3;
                }
                else {
                    final_encoding[i] = encode_index;
                }
            }
            
            printf("%d ", new_color_table_size);
            color_table_size = new_color_table_size;
            free(color_table);
            color_table = malloc(color_table_size * sizeof(int));
            memcpy(color_table, new_color_table, color_table_size*sizeof(int));
            free(new_color_table);
        }
        
        if (rflag) {
            printf("%s %d %d %d", font_msg, font_size, font_row, font_col);
            
        }

        if (output_sbu_flag) {
            fprintf(output_file, "SBU\n");
            fprintf(output_file, "%d %d\n", width, height);
            fprintf(output_file, "%d\n", color_table_size/3);

            for (int i = 0; i < color_table_size; i++) {
                fprintf(output_file, "%d ", color_table[i]);
            }
            fprintf(output_file, "\n");

            for (int i = 0; i < (width*height); i++) {
                int counter = 1;
                while ((i + counter) < (width*height) && (final_encoding[i] == final_encoding[i + counter])) {
                    counter++;
                }
                if (counter > 1) {
                    fprintf(output_file, "*%d %d ", counter, final_encoding[i]);
                }
                else {
                    fprintf(output_file, "%d ", final_encoding[i]);
                }
                i += counter - 1;
            }
        }
        else if (output_ppm_flag) {
            fprintf(output_file, "P3\n%d %d\n255\n", width, height);
            for (int i = 0; i < (width * height); i++) {
                int color_index = final_encoding[i];
                int r = color_table[color_index * 3];
                int g = color_table[(color_index * 3) + 1];
                int b = color_table[(color_index * 3) + 2];
                fprintf(output_file, "%d %d %d ", r,g,b);
                if ((i + 1) % width == 0) {
                    fprintf(output_file, "\n");
                }
            }
            fprintf(output_file,"\n");
            fclose(output_file);
        }
        fclose(input_file);
        free(color_table);
        free(encoding);
        free(encoding_copy);
        free(final_encoding);
    }
    return 0;
}


int colortable_exists(int *table, int table_size, int r, int g, int b) {
    for (int i = 0; i < table_size; i += 3) {
        if (table[i] == r && table[i+1] == g && table[i+2] == b) {
            return i/3;
        }
    }
    return -1;
}


