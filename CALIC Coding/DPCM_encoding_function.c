#include "DPCM_encoding_function.h"

void Encode_Using_DPCM(char *in_PGM_filename_Ptr, int prediction_rule,
                       float *avg_abs_error_Ptr, float *std_abs_error_Ptr){
    // Image variables
    struct PGM_Image image;
    load_PGM_Image(&image, in_PGM_filename_Ptr);
    int height = image.height,
            width = image.width,
            MGV = image.maxGrayValue,
            row,
            col,
            mode_flag;

    // Prediction variables
    int predArray[height][width];
    int arrayError[MGV + 1];
    memset(arrayError, 0, (MGV + 1)*sizeof(int));
    int sumError = 0;
    char *filename;
    char filenameBuffer[256];
    FILE *filePointer, *errorPointer;

    // Open the file for writing and check if it was successful
    snprintf(filenameBuffer, sizeof(filenameBuffer), "%s.%d.DPCM", in_PGM_filename_Ptr, prediction_rule);
    if(!(filePointer = fopen(filenameBuffer, "wb"))) {
        perror("Error opening DPCM file");
        return;
    }
    snprintf(filenameBuffer, sizeof(filenameBuffer), "%s.%d.errors.csv", in_PGM_filename_Ptr, prediction_rule);
    if(!(errorPointer = fopen(filenameBuffer, "wb"))) {
        perror("Error opening errors file");
        return;
    }

    // Write the header to the file and set the initial prediction
    int initialPred = image.image[0][0] - 128;
    if(fprintf(filePointer, "P2\n%d %d\n%d\n%d\n%d ", width, height, MGV, prediction_rule, initialPred) < 0) {
        perror("Error writing to DPCM");
        fclose(filePointer);
        fclose(errorPointer);
        return;
    }

    // Top west row
    for(col = 1; col < width; col++) {
        predArray[0][col] =  image.image[0][col] - image.image[0][col-1];
        fprintf(filePointer, "%d ", predArray[0][col]);
    }

    // Second north row
    for(col = 0; col < width; col++) {
        predArray[1][col] =  image.image[1][col] - image.image[0][col];
        fprintf(filePointer, "%d ", predArray[1][col]);
    }

    // First and second north columns
    for(row = 2; row < height; row++) {
        for(col = 0; col < 2; col++) {
            predArray[row][col] = image.image[row][col] - image.image[row-1][col];
            fprintf(filePointer, "%d ", predArray[row][col]);
        }
    }

    // Last north column
    for(row = 2; row < height; row++) {
        predArray[row][width-1] = image.image[row][width-1] - image.image[row-1][width-1];
        fprintf(filePointer, "%d ", predArray[row][width-1]);
    }

    // Encode the rest of the image based on the prediction rule
    switch(prediction_rule) {

        // Prediction rule 1 (West)
        case 1:
            for(row = 2; row < height; row++) { // Loop through the image and encode the pixels
                for(col = 2; col < (width - 1); col++) {
                    predArray[row][col] =  image.image[row][col] - image.image[row][col-1]; // Calculate prediction
                    fprintf(filePointer, "%d ", predArray[row][col]); // Write to file
                }
            }
            break;

            // Prediction rule 2 (North)
        case 2:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    predArray[row][col] =  image.image[row][col] - image.image[row-1][col];
                    fprintf(filePointer, "%d ", predArray[row][col]);
                }
            }
            break;

            // Prediction rule 3 (Average of West and North)
        case 3:
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {
                    predArray[row][col] =  image.image[row][col] - ((image.image[row][col-1] / 2) + (image.image[row-1][col] / 2));
                    fprintf(filePointer, "%d ", predArray[row][col]);
                }
            }
            break;

        // CALIC prediction
        case 4:
            mode_flag = 1; // Start in binary mode
            // iterate through image
            for(row = 2; row < height; row++) {
                for(col = 2; col < (width - 1); col++) {

                    float predFloat; // Prediction float
                    int w = image.image[row][col-1], // West
                    ww = image.image[row][col-2], // West west
                    n = image.image[row-1][col], // North
                    nw = image.image[row-1][col-1], // North west
                    ne = image.image[row-1][col+1], // North east
                    nn = image.image[row-2][col], // North north
                    nne = image.image[row-2][col+1], // North north east

                    dh = abs(w - ww) + abs(n - nw) + abs(ne - n), // Horizontal difference
                    dv = abs(w - nw) + abs(n - nn) + abs(ne - nne), // Vertical difference

                    flag = 1, // Mode flag
                    flagCheck[6] = { ww, n, nw, ne, nn, nne }, // Mode check array
                    binary = 1, // Binary flag
                    binaryCheck[2]; // Binary check array

                    // Check if CALIC should be in binary mode or continuous mode
                    if(mode_flag) {
                        binaryCheck[0] = w;
                        binary = 1;

                        // Check if binary mode should be exited
                        for(int i = 0; i < 6; i++) {
                            if(binary != 2) {
                                if(flagCheck[i] != binaryCheck[0]) { // If mode check is not binary check 0
                                    binaryCheck[1] = flagCheck[i]; // Set binary check 1 to mode check
                                    binary++; // Increment binary
                                }
                                // Exit binary mode if >2 values found
                            } else if(binaryCheck[0] != flagCheck[i] && binaryCheck[1] != flagCheck[i]) {
                                if(flag == 0) {
                                    fprintf(filePointer, "2 "); // Send 2 to exit binary mode
                                    flag = 1;
                                }
                                mode_flag = 0;
                                break;
                            }
                            // Start binary mode if loop is done
                            if(i == 5) {
                                flag = 0;
                            }
                        }
                    }

                    // CALIC binary mode
                    if(flag == 0) {
                        printf("1");
                        // Match
                        if(image.image[row][col] == w)
                            predArray[row][col] = 0;
                        else {
                            // Exit binary mode
                            if(image.image[row][col] != binaryCheck[0] && image.image[row][col] != binaryCheck[1]) {
                                flag = 1;
                                mode_flag = 0;
                                fprintf(filePointer, "2 "); // Send 2 to exit binary mode
                                // Other value
                            } else {
                                predArray[row][col] = 1;
                            }
                        }
                    }

                    // CALIC continuous-tone mode
                    if(flag == 1) {

                        // Predictions
                        if(dv - dh > 80)
                            predArray[row][col] = w;

                        else if(dh - dv > 80)
                            predArray[row][col] = n;

                        else {
                            predFloat = (w + n)/2.0 + (ne - nw)/4.0; // Prediction float, used for rounding
                            if(dv - dh > 32)
                                predArray[row][col] = round((1.0/2.0) * predFloat + (1.0/2.0) * w);
                            else if(dh - dv > 32)
                                predArray[row][col] = round((1.0/2.0) * predFloat + (1.0/2.0) * n);
                            else if(dv - dh > 8)
                                predArray[row][col] = round((3.0/4.0) * predFloat + (1.0/4.0) * w);
                            else if(dh - dv > 8)
                                predArray[row][col] = round((3.0/4.0) * predFloat + (1.0/4.0) * n);
                            else
                                predArray[row][col] = round(predFloat);
                        }

                        predArray[row][col] = image.image[row][col] - predArray[row][col]; // Calculate prediction
                    }
                    fprintf(filePointer, "%d ", predArray[row][col]); // Write to file
                }
            }
            break;

        // Case for incorrect prediction rule
        default:
            printf("Incorrect prediction rule\n");
    }

    // Calculate the mean and standard deviation of the error
    float meanError = 0.0;
    int valError;
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            valError = abs(predArray[row][col]);
            arrayError[valError]++;
            sumError += valError;
            meanError = ((meanError * (row * width + col)) + valError) / (row * width + col + 1);
        }
    }

    // Calculate the standard deviation
    float stdError = 0.0,
            tempDiff;
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            tempDiff = abs(predArray[row][col]) - meanError;
            stdError += tempDiff * tempDiff;
        }
    }
    stdError = sqrt(stdError / (height * width));

    // Write the error values to the error file
    for(int i = 0; i < MGV+1; i++) {
        if(arrayError[i] != 0) {
            fprintf(errorPointer, "%d,%d\n", i, arrayError[i]);
        }
    }

    // Close the files
    fclose(filePointer);
    fclose(errorPointer);

    // Set the pointers to the mean and standard deviation of the error
    *avg_abs_error_Ptr = meanError;
    *std_abs_error_Ptr = stdError;

    // Free the image
    free_PGM_Image(&image);
}


