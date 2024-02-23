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
    int i;
    int o_count = 0;
    int i_count = 0;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i+1 < argc && argv[i][0] != '-') {
            o_count += 1;
        }
        else if (strcmp(argv[i], "-i") == 0 && i+1 < argc && argv[i][0] != '-') {
            i_count += 1;
        }
    }

    if (o_count == 1 && i_count == 1) {
        return 0;
    }
    else {
        return MISSING_ARGUMENT;
    }
}
