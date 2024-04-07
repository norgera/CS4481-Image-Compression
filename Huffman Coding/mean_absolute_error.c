#include "mean_absolute_error.h"

float mean_absolute_error(char *file_name_1_ptr, char *file_name_2_ptr) {
    struct PGM_Image image1, image2;

    // Load the two images
    if (load_PGM_Image(&image1, file_name_1_ptr) == -1 || load_PGM_Image(&image2, file_name_2_ptr) == -1) {
        printf("Error loading images.\n");
        return 0; // Error loading images
    }

    // Check if the images are of the same size
    if (image1.width != image2.width || image1.height != image2.height) {
        free_PGM_Image(&image1);
        free_PGM_Image(&image2);
        return 0; // Images are not of the same size
    }

    // Scale images if their max gray values are not the same
    if (image1.maxGrayValue != image2.maxGrayValue) {
        // Assuming image1 has the smaller maxGrayValue. If not, swap image1 and image2.
        if (image1.maxGrayValue > image2.maxGrayValue) {
            struct PGM_Image temp = image1;
            image1 = image2;
            image2 = temp;
        }
        // Scale image1 to have the same maxGrayValue as image2
        for (int i = 0; i < image1.height; i++) {
            for (int j = 0; j < image1.width; j++) {
                image1.image[i][j] = (image1.image[i][j] * image2.maxGrayValue) / image1.maxGrayValue;
            }
        }
    }

    // Calculate Mean Absolute Error
    long int total_diff = 0;
    for (int i = 0; i < image1.height; i++) {
        for (int j = 0; j < image1.width; j++) {
            total_diff += fabs((float)image1.image[i][j] - (float)image2.image[i][j]);
        }
    }
    float mae = (float)total_diff / (image1.width * image1.height);

    // Free the images
    free_PGM_Image(&image1);
    free_PGM_Image(&image2);

    return mae;
}

