#include "DPCM_decoding_function.h"

// Function to decode a DPCM image, similar to the encoding function
void Decode_Using_DPCM (char *in_filename_Ptr) {

    // Initialize the variables
    char c, *filename;
    int row, col, width, height, MGV, predRule, mode_flag;
    struct PGM_Image image;
    FILE *dpcmPointer =  fopen(in_filename_Ptr, "rb");

    // Check if the file is valid
    if(fgetc(dpcmPointer) != 'P') {
        printf("Invalid PGM image\n");
        fclose(dpcmPointer);
    }
    c = fgetc(dpcmPointer);
    if(c != '2' && c != '5') {
        printf("Invalid PGM image\n");
        fclose(dpcmPointer);
    }

    // Initialize the image variables
    width = geti(dpcmPointer);
    height = geti(dpcmPointer);
    MGV = geti(dpcmPointer);
    predRule = geti(dpcmPointer);
    int predArray[height][width];

    // Create the image and set the first pixel
    create_PGM_Image(&image, width, height, MGV);
    fscanf(dpcmPointer, "%d ", &predArray[0][0]);
    image.image[0][0] = predArray[0][0] + 128;

    // Decode the rows and columns of the image
    decodeTopWestRow(dpcmPointer, &image, predArray, width);
    decodeSecondNorthRow(dpcmPointer, &image, predArray, width);
    decodeFirstAndSecondNorthColumns(dpcmPointer, &image, predArray, height);
    decodeLastNorthColumn(dpcmPointer, &image, predArray, width, height);


    // Decode the rest of the image based on the prediction rule
    switch(predRule) {

        // Prediction rule 1 (West)
        case 1:
            // Loop through the image and decode the pixels
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    fscanf(dpcmPointer, "%d ", &predArray[row][col]); // Read the prediction from the file
                    image.image[row][col] = predArray[row][col] + image.image[row][col-1]; // Add the prediction to the pixel
                }
            }
            break;

        // Prediction rule 2 (North)
        case 2:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    fscanf(dpcmPointer, "%d ", &predArray[row][col]);
                    image.image[row][col] = predArray[row][col] + image.image[row-1][col];
                }
            }
            break;

        // Prediction rule 3 (Average of West and North)
        case 3:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    fscanf(dpcmPointer, "%d ", &predArray[row][col]);
                    image.image[row][col] = predArray[row][col] + ((image.image[row][col-1] / 2) + (image.image[row-1][col] / 2));
                }
            }
            break;

        // Prediction rule 4 (CALIC)
        case 4:
            mode_flag = 1; // Set the mode flag to 1 and loop through the image
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    float prediction_fl;

                    // Calculate the variables needed for the prediction
                    int w = image.image[row][col-1],
                            ww = image.image[row][col-2],
                            n = image.image[row-1][col],
                            nw = image.image[row-1][col-1],
                            ne = image.image[row-1][col+1],
                            nn = image.image[row-2][col],
                            nne = image.image[row-2][col+1],
                            dh = abs(w - ww) + abs(n - nw) + abs(ne - n),
                            dv = abs(w - nw) + abs(n - nn) + abs(ne - nne),
                            mode = 1, // Set the mode to 1

                            // Create the variables needed for the binary mode
                            mode_check[6] = { ww,
                                              n,
                                              nw,
                                              ne,
                                              nn,
                                              nne },
                            binary = 1,
                            binary_check[2],
                            prediction;

                    fscanf(dpcmPointer, "%d ", &predArray[row][col]); // Read the prediction from the file

                    // Check if the mode is binary
                    if(mode_flag) {
                        binary_check[0] = w; // Set the first binary check to the west pixel
                        binary = 1;
                        for(int i = 0; i < 6; i++) {
                            // Check if the mode is binary or continuous
                            if(binary != 2) {
                                if(mode_check[i] != binary_check[0]) {
                                    binary_check[1] = mode_check[i];
                                    binary++;
                                }
                                // If the mode is not binary, set the mode to 1 and read the next prediction
                            } else if(binary_check[0] != mode_check[i] && binary_check[1] != mode_check[i]) {
                                if(mode == 0) {
                                    mode = 1;
                                    fscanf(dpcmPointer, "%d ", &predArray[row][col]);
                                }
                                mode_flag = 0; // Set the mode flag to 0 if the mode is not binary
                                break;
                            }
                            // If the mode is binary, set the mode to 0
                            if(i == 5) {
                                mode = 0;
                            }
                        }
                    }

                    // Check if the mode is continuous
                    if(mode == 0) {
                        if(predArray[row][col] == 0)
                            image.image[row][col] = w; // Set the pixel to the west pixel
                        else if(predArray[row][col] == 1) {
                            if(w != binary_check[0])
                                image.image[row][col] = binary_check[0]; // if the west pixel is not the same as binary check 0, set the pixel to binary check 0
                            else
                                image.image[row][col] = binary_check[1]; // if the west pixel is the same as binary check 0, set the pixel to binary check 1
                        } else if(predArray[row][col] == 2) {
                            mode = 1;
                            mode_flag = 0;
                            fscanf(dpcmPointer, "%d ", &predArray[row][col]); // Read the next prediction
                        }
                    }

                    // Check if the mode is binary continuous
                    if(mode == 1) {
                        if(dv - dh > 80)
                            prediction = w;
                        else if(dh - dv > 80)
                            prediction = n;
                        else {
                            prediction_fl = (w + n)/2.0 + (ne - nw)/4.0;
                            if(dv - dh > 32)
                                prediction = round((1.0/2.0) * prediction_fl + (1.0/2.0) * w);
                            else if(dh - dv > 32)
                                prediction = round((1.0/2.0) * prediction_fl + (1.0/2.0) * n);
                            else if(dv - dh > 8)
                                prediction = round((3.0/4.0) * prediction_fl + (1.0/4.0) * w);
                            else if(dh - dv > 8)
                                prediction = round((3.0/4.0) * prediction_fl + (1.0/4.0) * n);
                            else
                                prediction = round(prediction_fl);
                        }

                        image.image[row][col] = prediction + predArray[row][col]; // Add the prediction to the pixel
                    }
                }
            }
            break;
            // If the prediction rule is not 1-4, display an error
        default:
            printf("Incorrect prediction rule\n");
    }

    fclose(dpcmPointer); // Close the file

    // Save the image
    char file[strlen(in_filename_Ptr) + 5];
    sprintf(file, "%s.pgm", in_filename_Ptr);
    save_PGM_Image(&image, file, 0);
    free_PGM_Image(&image);

}

//Helper functions to decode the rows and columns of the image
void decodeTopWestRow(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int width) {
    for(int col = 1; col < width; col++) {
        fscanf(dpcmPointer, "%d ", &predArray[0][col]);
        image->image[0][col] = predArray[0][col] + image->image[0][col-1];
    }
}

void decodeSecondNorthRow(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int width) {
    for(int col = 0; col < width; col++) {
        fscanf(dpcmPointer, "%d ", &predArray[1][col]);
        image->image[1][col] = predArray[1][col] + image->image[0][col];
    }
}

void decodeFirstAndSecondNorthColumns(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int height) {
    for(int row = 2; row < height; row++) {
        for(int col = 0; col < 2; col++) {
            fscanf(dpcmPointer, "%d ", &predArray[row][col]);
            image->image[row][col] = predArray[row][col] + image->image[row-1][col];
        }
    }
}

void decodeLastNorthColumn(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int width, int height) {
    for(int row = 2; row < height; row++) {
        fscanf(dpcmPointer, "%d ", &predArray[row][width-1]);
        image->image[row][width-1] = predArray[row][width-1] + image->image[row-1][width-1];
    }
}






