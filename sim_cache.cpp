#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include "Main1.h"

cache_params params;
char *trace;

int main (int argc, char* argv[]) {
    if (argc != 9)            // Checks if correct number of inputs have been given. Throw error and exit if wrong
    {
        printf("Error: Expected inputs:8 Given inputs:%d\n", argc - 1);
        exit(EXIT_FAILURE);
    }

    //strtoul() converts char* to unsigned long. It is included in <stdlib.h>
    params.block_size = strtoul(argv[1], NULL, 10);
    params.l1_size = strtoul(argv[2], NULL, 10);
    params.l1_assoc = strtoul(argv[3], NULL, 10);
    params.l2_size = strtoul(argv[4], NULL, 10);
    params.l2_assoc = strtoul(argv[5], NULL, 10);
    params.replacement_policy = strtoul(argv[6], NULL, 10);
    params.inclusive = strtoul(argv[7], NULL, 10);
    trace = argv[8];

    Main1 m = Main1(params, trace);
    m.start();

    return 0;
}