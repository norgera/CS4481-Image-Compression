#include <time.h>
#include "DPCM_encoding_function.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("wrong format\n", argv[0]);
        return 0;
    }

    // Parsing command line arguments
    char *PGM_image = argv[1];
    unsigned int prediction_rule_code = atoi(argv[2]);

    // Variables for storing statistics
    float error_avg, error_std;
    double compression_time;

    // Start timing
    clock_t startTime = clock();

    // Encode the image using DPCM
    Encode_Using_DPCM(PGM_image, prediction_rule_code, &error_avg, &error_std);

    // Stop timing
    clock_t endTime = clock();
    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Print the results
    printf("APE average: %.2f\nAPE std: %.2f\nCompression time: %.2f\n", error_avg, error_std, totalTime);
}

