#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lz77_encoding_function.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <PGM image> <buffer size>\n", argv[0]);
        return 1;
    }

    // Parsing command line arguments
    char *inputFile = argv[1];
    unsigned int bufferSize = atoi(argv[2]);

    // Variables for storing statistics
    float avgOffset, stdOffset, avgMatchLength, stdMatchLength;

    // Start timing
    clock_t startTime = clock();

    // Perform LZ77 encoding
    Encode_Using_LZ77(inputFile, bufferSize, &avgOffset, &stdOffset, &avgMatchLength, &stdMatchLength);

    // Stop timing
    clock_t endTime = clock();
    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Print the results
    printf("Avg Offset: %f, Std Offset: %f, Avg Match Length: %f, Std Match Length: %f\n", avgOffset, stdOffset, avgMatchLength, stdMatchLength);
    printf("Compression Time: %f seconds\n", totalTime);

    return 0;
}
