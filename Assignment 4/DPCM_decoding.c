#include <time.h>
#include "DPCM_decoding_function.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("wrong format\n", argv[0]);
        return 0;
    }

    char *DPCM_image = argv[1];

    // Start timing
    clock_t startTime = clock();

    // Decode the image using DPCM
    Decode_Using_DPCM(DPCM_image);

    // Stop timing
    clock_t endTime = clock();
    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Report decompression time
    printf("Decompression time: %.2f\n", totalTime);

    return 0;
}

