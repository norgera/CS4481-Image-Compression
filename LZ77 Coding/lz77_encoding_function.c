#include "lz77_encoding_function.h"

void Encode_Using_LZ77(char *in_PGM_filename_Ptr, unsigned int searching_buffer_size,  float *avg_offset_Ptr, float *std_offset_Ptr, float *avg_length_Ptr, float *std_length_Ptr) {


    struct PGM_Image pgm;
    load_PGM_Image(&pgm, in_PGM_filename_Ptr); // Load the PGM image
    int *pixel_array = malloc(pgm.height * pgm.width * sizeof(int)), pixel_count = 0; // Create the pixel array

    // flatten the 2D array into a 1D array
    for(int row = 0; row < pgm.height; row++) {
        for(int col = 0; col < pgm.width; col++) {
            pixel_array[pixel_count] = pgm.image[row][col]; // Set the pixel value
            pixel_count++; // Increment the pixel count
        }
    }

    // Create the arrays
    int *offset = calloc(pixel_count, sizeof(int));
    int *m_length = calloc(pixel_count, sizeof(int));
    int *next = calloc(pixel_count, sizeof(int));
    int buffer_size = 0, matches, tk;

    // Loop through the pixel array
    for(tk = 0; buffer_size < pixel_count; tk++) {
        // Set the matching length to 0
        for(int i = 1; i <= searching_buffer_size && i <= buffer_size; i++) {
            matches = 0;
            // Find the number of matches
            while(pixel_array[buffer_size + matches] == pixel_array[buffer_size - i + matches] && buffer_size + matches < pixel_count) {
                matches++;
            }
            // If the number of matches is greater than the current matching length, update the matching length and offset
            if(matches > m_length[tk]) {
                m_length[tk] = matches;
                offset[tk] = i;
            }
        }
        // If the matching length is greater than the buffer size, set the matching length to the buffer size
        buffer_size += m_length[tk];
        next[tk] = pixel_array[buffer_size];
        buffer_size++;
    }

    int buffer = (int)(ceil(log10(searching_buffer_size)) + 1); // Calculate the buffer size

    // Create the file names
    char lzFileName[strlen(in_PGM_filename_Ptr) + buffer + 5];
    char offsetsFile[strlen(in_PGM_filename_Ptr) + buffer + 14];
    char lengthsFile[strlen(in_PGM_filename_Ptr) + buffer + 14];
    sprintf(lzFileName, "%s.%d.lz", in_PGM_filename_Ptr, searching_buffer_size);
    sprintf(offsetsFile, "%s.%d.offsets.csv", in_PGM_filename_Ptr, searching_buffer_size);
    sprintf(lengthsFile, "%s.%d.lengths.csv", in_PGM_filename_Ptr, searching_buffer_size);

    // Open the files
    FILE *lzPointer = fopen(lzFileName, "wb");
    FILE *offsetsFilePointer = fopen(offsetsFile, "wb");
    FILE *lengthsFilePointer = fopen(lengthsFile, "wb");
    fprintf(lzPointer, "P2\n%d %d\n%d\n%d %d\n", pgm.width, pgm.height, pgm.maxGrayValue, searching_buffer_size, tk);
    int *offset_freq = calloc(tk, sizeof(int)); // Create the offset frequency array
    int *length_freq = calloc(tk, sizeof(int)); // Create the matching length frequency array
    int offset_sum = 0, length_sum = 0; // Set the offset and matching length sums to 0


    // Write the data to the lz file
    for(int i = 0; i < tk; i++) {
        fprintf(lzPointer, "%d ", offset[i]); // Write the offset to the file
        offset_freq[offset[i]]++;
    }
    for(int i = 0; i < tk; i++) {
        fprintf(lzPointer, "%d ", m_length[i]); // Write the matching length to the file
        length_freq[m_length[i]]++;
    }
    for(int i = 0; i < tk; i++)
        fprintf(lzPointer, "%d ", next[i]); // Write the next value to the file


    // Write the frequency of offsets and matching lengths to the csv files
    for(int i = 0; i < tk; i++) {
        if(offset_freq[i] != 0) {
            fprintf(offsetsFilePointer, "%d,%d\n", i, offset_freq[i]); // Write the offset frequency to the file
        }
        if(length_freq[i] != 0) {
            fprintf(lengthsFilePointer, "%d,%d\n", i, length_freq[i]); // Write the length frequency to the file
        }

        // Update the offset and matching length sums
        offset_sum += offset[i];
        length_sum += m_length[i];
    }
    // Close the files
    fclose(lzPointer);
    fclose(offsetsFilePointer);
    fclose(lengthsFilePointer);

    // Calculate the average and standard deviation of the offsets and match lengths
    float offset_mean, offset_std = 0.0, length_mean, length_std = 0.0;
    offset_mean = (float) offset_sum / tk;
    length_mean = (float) length_sum / tk;

    for(int i = 0; i < tk; i++) {
        offset_std += pow(offset[i] - offset_mean, 2);
        length_std += pow(m_length[i] - length_mean, 2);
    }

    // Calculate the standard deviation
    offset_std = sqrt(offset_std / tk);
    length_std = sqrt(length_std / tk);
    *avg_offset_Ptr = offset_mean;
    *avg_length_Ptr = length_mean;
    *std_offset_Ptr = offset_std;
    *std_length_Ptr = length_std;

    // Free memory
    free_PGM_Image(&pgm);
    free(pixel_array);
    free(offset);
    free(m_length);
    free(next);
    free(offset_freq);
    free(length_freq);
}