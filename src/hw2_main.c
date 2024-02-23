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
    int o_count = 0;
    int i_count = 0;

    while ((c = getopt(argc, argv, "o:i:")) != -1) {
        switch(c) {
            case 'o':
                o_count += 1;
                break;
            case 'i':
                i_count += 1;
                break;
        }
    }

    if (o_count == 1 && i_count == 1) {
        return 0;
    }
    else {
        return MISSING_ARGUMENT;
    }
}
