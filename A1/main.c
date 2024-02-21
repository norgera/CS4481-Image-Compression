/**
 * @author Nathan Orgera
 * @date 7/02/2024
 * @brief This program is designed to generate PBM, PGM, and PPM images based on the user's input.
 * The program will generate the images and save them to the current directory.
 */

#include "libpnm.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Function to draw a diagonal line for a horizontal image
void draw_horizontal(struct PBM_Image *pbmImage) {
    int width = pbmImage->width;
    int height = pbmImage->height;
    float aspectRatio = (float)width / height;

    // Loop through the pixels in the image
    for (int x = 0; x < width; x++) {

        // Calculate the y values for the diagonal line
        int diagonalY1 = (int)(x / aspectRatio);
        int diagonalY2 = (int)((width - x - 1) / aspectRatio);

        // if the pixel is within the bounds of the image, set it to black
        if (diagonalY1 < height) {
            pbmImage->image[diagonalY1][x] = BLACK;
        }
        if (diagonalY2 < height && diagonalY2 >= 0) {
            pbmImage->image[diagonalY2][x] = BLACK;
        }
    }
}

// Function to draw a diagonal line for vertical PBM images
void draw_vertical(struct PBM_Image *pbmImage) {
    int width = pbmImage->width;
    int height = pbmImage->height;
    float aspectRatio = (float)height / width;

    // Loop through the pixels in the image
    for (int y = 0; y < height; y++) {

        // Calculate the x values for the diagonal line
        int diagonalX1 = (int)(y / aspectRatio);
        int diagonalX2 = (int)((height - y - 1) / aspectRatio);

        // if the pixel is within the bounds of the image, set it to black
        if (diagonalX1 < width) {
            pbmImage->image[y][diagonalX1] = BLACK;
        }
        if (diagonalX2 < width && diagonalX2 >= 0) {
            pbmImage->image[y][diagonalX2] = BLACK;
        }
    }
}

// Function to generate and save a PBM image
int generate_pbm(int width, int height, char *outputFile, bool raw) {

    // Validate the width and height constraints for PBM images
    if (width % 4 != 0 || width < 4 || height % 4 != 0 || height < 4) {
        printf("Error: width and height must be divisible by 4 and greater than or equal to 4.\n");
        return 0;
    }


    struct PBM_Image pbmImage;

    // Error Handling
    if (create_PBM_Image(&pbmImage, width, height) != 0) {
        printf("Error: Unable to create PBM image.\n");
        return 0;
    }

    // Fill image with black pixels
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pbmImage.image[y][x] = BLACK;
        }
    }

    // Draw the white rectangle in the middle
    for (int y = height / 4; y < 3 * height / 4; y++) {
        for (int x = width / 4; x < 3 * width / 4; x++) {
            pbmImage.image[y][x] = WHITE;
        }
    }

    // Check if image is vertical or horizontal and draw accordingly
    if (width >= height) {
        draw_horizontal(&pbmImage);
    } else {
        draw_vertical(&pbmImage);
    }

    // Save and free the image
    if (save_PBM_Image(&pbmImage, outputFile, raw) != 0) {
        printf("Error: Unable to save PBM image.\n");
        free_PBM_Image(&pbmImage);
        return 0;
    }

    free_PBM_Image(&pbmImage);

    return 0;
}



// Function to generate and save a PGM image
int generate_pgm(int width, int height, char *outputFile, bool raw) {


    //Error Handling
    if (width < 4 || height < 4 || width % 4 != 0 || height % 4 != 0) {
        printf("Error: The image width and height must be divisible by 4 and greater than or equal to 4.\n");
        return 0;
    }
    struct PGM_Image pgmImage;
    if (create_PGM_Image(&pgmImage, width, height, 255) != 0) {
        printf("Error: Unable to create PGM image.\n");
        return 0;
    }

    // Draw the gradient triangles
    int half_width = width / 2;
    int half_height = height / 2;
    int quarter_width = width / 4;
    int quarter_height = height / 4;
    int x, y;
    float gray_value;

    // loop through the pixels in the image
    for (y = quarter_height; y < quarter_height + half_height; y++) {
        for (x = quarter_width; x < quarter_width + half_width; x++) {

            // Calculate the gray value for the pixels
            if (x < width / 2) { // Left side
                gray_value = (float)(x - quarter_width) / (half_width / 2) * 255;
            } else { // Right side
                gray_value = (float)(3 * quarter_width - x) / (half_width / 2) * 255;
            }
            if (y < height / 2) { // Top side
                gray_value = fmin(gray_value, (float)(y - quarter_height) / (half_height / 2) * 255);
            } else { // Bottom side
                gray_value = fmin(gray_value, (float)(3 * quarter_height - y) / (half_height / 2) * 255);
            }

            // Set the pixel color
            pgmImage.image[y][x] = 255 - (unsigned char)gray_value;
        }
    }

    // Save and free the image
    if (save_PGM_Image(&pgmImage, outputFile, raw) != 0) {
        printf("Error: Unable to save PGM image.\n");
        free_PGM_Image(&pgmImage);
        return 0;
    }
    free_PGM_Image(&pgmImage);

    return 0;
}


// Function to generate and save a PPM image
void generate_ppm(int width, int height, char *outputFile, bool raw) {

    //Error Handling
    if (width % 6 != 0 || width < 6 || height % 6 != 0 || height < 6) {
        printf("Error: The image width and height must be divisible by 6 and greater than or equal to 6.\n");
        return;
    }

    struct PPM_Image ppmImage;

    create_PPM_Image(&ppmImage, width, height, 255);

    // Draw the gradient triangles
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int red = 0, green = 0, blue = 0;

            // Calculate the color values for the pixels
            if (y < height / 2) { // Upper half
                if (x < width / 3) { // Red to White
                    red = 255;
                    green = blue = (y * 255) / (height / 2);
                } else if (x < (2 * width) / 3) { // White to Green
                    green = 255;
                    red = blue = 255 - (y * 255) / (height / 2);
                } else { // Blue to White
                    blue = 255;
                    red = green = (y * 255) / (height / 2);
                }
            } else { // Lower half
                if (x < width / 2) { // Black to White
                    red = green = blue = (y - height / 2) * 255 / (height / 2);
                } else { // White to Black
                    red = green = blue = 255 - ((y - height / 2) * 255 / (height / 2));
                }
            }

            // Set the pixel color
            ppmImage.image[y][x][RED] = red;
            ppmImage.image[y][x][GREEN] = green;
            ppmImage.image[y][x][BLUE] = blue;
        }
    }

    save_PPM_Image(&ppmImage, outputFile, raw);

    // Convert the PPM image to a PGM image
    struct PGM_Image pgmImage;
    convert_PPM_to_PGM_using_average(&ppmImage, &pgmImage);

    // filenames for the gray images
    char pgmFilename[100];
    sprintf(pgmFilename, "%s_gray.pgm", outputFile);
    save_PGM_Image(&pgmImage, pgmFilename, 1);

    // Free the images
    free_PGM_Image(&pgmImage);
    free_PPM_Image(&ppmImage);
}


int main(int argc, char *argv[]) {
    // Validate the correct number of arguments
    if (argc != 6) {
        printf("Usage: %s image_type_code image_width image_height output_image_name image_format_code\n", argv[0]);
        return 0;
    }

    // Parse the command line arguments
    int imageTypeCode = atoi(argv[1]);
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    char *outputFile = argv[4];
    bool format = atoi(argv[5]) != 0; // 0 for ASCII, 1 for raw


    // Switch statement to generate the image based on the image type code
    switch (imageTypeCode) {
        case 1: // PBM image
            generate_pbm(width, height, outputFile, format);
            break;
        case 2: // PGM image
            generate_pgm(width, height, outputFile, format);
            break;
        case 3: // PPM image
            generate_ppm(width, height, outputFile, format);
            break;
        default:
            printf("Invalid type code. Use 1 for PBM, 2 for PGM, or 3 for PPM.\n");
            return 0;
    }
    return 0;
}