#include "lz77_decoding_function.h"

void Decode_Using_LZ77(char *in_compressed_filename_Ptr) {

    char h;
    int width, height, MGV, row, col, searching_buffer_size, tokens, num_pixels, buffer_size = 0;
    struct PGM_Image pgmImage;
    FILE *lzPointer =  fopen(in_compressed_filename_Ptr, "rb");

    // Make sure the first char is P
    if(fgetc(lzPointer) != 'P') {
        printf("Invalid PGM image: missing P");
        fclose(lzPointer);
    }

    // Make sure the second char is either a 2 or 5
    h = fgetc(lzPointer);
    if(h != '2' && h != '5') {
        printf("Invalid PGM image: missing 2 or 5");
        fclose(lzPointer);
    }

    // Get the values from the file and set variables
    width = geti(lzPointer);
    height = geti(lzPointer);
    MGV = geti(lzPointer);
    searching_buffer_size = geti(lzPointer);
    tokens = geti(lzPointer);
    create_PGM_Image(&pgmImage, width, height, MGV);
    num_pixels = width * height;
    int *offset = calloc(tokens, sizeof(int));
    int *m_length = calloc(tokens, sizeof(int));
    int *next = calloc(tokens, sizeof(int));
    int *p_array = malloc(num_pixels * sizeof(int));

    // Get the values from the file and set the arrays
    for(int i = 0; i < tokens; i++)
        offset[i] = geti(lzPointer);
    for(int i = 0; i < tokens; i++)
        m_length[i] = geti(lzPointer);
    for(int i = 0; i < tokens; i++)
        next[i] = geti(lzPointer);
    fclose(lzPointer);

    // Decode the data from the arrays
    for(int tk = 0; tk < tokens; tk++) {
        // Add matching length number of items
        for(int i = 0; i < m_length[tk]; i++) {
            p_array[buffer_size] = p_array[buffer_size - offset[tk]];
            buffer_size++;
        }
        p_array[buffer_size] = next[tk]; // Add the next value
        buffer_size++; // Increment the buffer size
    }

    // Loop through the pixel array
    num_pixels = 0; // Reset pixel counter
    for(row = 0; row < height; row++) {
        for(col = 0; col < width; col++) {
            pgmImage.image[row][col] = p_array[num_pixels]; // Set the pixel value
            num_pixels++; // Increment pixel counter
        }
    }

    // Save image
    char lzFileName[strlen(in_compressed_filename_Ptr) + 5];
    sprintf(lzFileName, "%s.pgm", in_compressed_filename_Ptr);
    save_PGM_Image(&pgmImage, lzFileName, 0);

    // Free memory
    free_PGM_Image(&pgmImage);
    free(offset);
    free(m_length);
    free(next);
    free(p_array);
}