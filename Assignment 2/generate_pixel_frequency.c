#include "generate_pixel_frequency.h"

// generates the pixel frequency
long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, 
	int *number_of_non_zero_values_in_the_frequency_array){

	long int* pixels = calloc(MAX_GRAY_VALUE + 1, sizeof(long int)); // Initialize the pixels array

    // Generate the pixel frequency
	for(int i = 0; i < input_pgm_image->height; i++){
		for(int j = 0; j < input_pgm_image->width; j++){

			int cur = input_pgm_image->image[i][j]; // Get the current pixel value

            // Increment the pixel frequency and the number of non-zero values in the frequency array
			if(pixels[cur] == 0){
				(*number_of_non_zero_values_in_the_frequency_array)++;
			}
			pixels[cur]++;
		}
	}
	return pixels;
}
