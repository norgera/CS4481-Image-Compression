#include "mean_absolute_error.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PGM file name 1> <PGM file name 2>\n", argv[0]);
        return 0;
    }

    char *pgmFileName1 = argv[1];
    char *pgmFileName2 = argv[2];

    // Call mean_absolute_error and store its return value
    float mae = mean_absolute_error(pgmFileName1, pgmFileName2);

    // Check for error conditions returned by mean_absolute_error
    if (mae == -1) {
        fprintf(stderr, "Error: The images cannot be compared. They might have different dimensions or failed to load.\n");
    } else {
        printf("Mean Absolute Error (MAE) between the two images: %f\n", mae);
    }

    return 0;
}

