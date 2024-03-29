#include <stdio.h>
#include <time.h>
#include "lz77_decoding_function.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <compressed_file.lz>\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];

    // Start timing
    clock_t startTime = clock();

    // Perform LZ77 decoding
    Decode_Using_LZ77(inputFile);

    // Stop timing
    clock_t endTime = clock();
    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Report decompression time
    printf("Decompression completed in %f seconds.\n", totalTime);

    return 0;
}
