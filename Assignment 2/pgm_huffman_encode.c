#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "libpnm.h"
#include "generate_pixel_frequency.h"
#include "generate_huffman_nodes.h"
#include "huffman_encode_image.h"
#include "store_huffman_encoded_data.h"

int main(int argc, char *argv[]){

	if(argc != 3){
		printf("Please ensure you have entered two arguments. You entered: %d\n", argc - 1);
		exit(0);
	}

	// loading the pgm image
	struct PGM_Image* image = malloc(sizeof(struct PGM_Image)); 
	if(load_PGM_Image(image, argv[1]) == -1){
		printf("Please ensure you have entered a valid input file.\n");
		exit(0);
	}

	// generating pixel frequencies
	int number_of_non_zero_values_in_the_frequency_array = 0;
	long int *frequencies = generate_pixel_frequency(image, 
		&number_of_non_zero_values_in_the_frequency_array);

	// generating huffman nodes
	struct node* nodes = generate_huffman_nodes(frequencies, MAX_GRAY_VALUE, 
		number_of_non_zero_values_in_the_frequency_array);

	// generating the encoding
	unsigned long int length_of_encoded_array = 0;
	unsigned char* encoding = huffman_encode_image(image, nodes, 
		number_of_non_zero_values_in_the_frequency_array - 1, &length_of_encoded_array);

	// storing the encoding
	store_huffman_encoded_data(argv[2], image->width, image->height, MAX_GRAY_VALUE, 
		number_of_non_zero_values_in_the_frequency_array - 1, nodes, 
		length_of_encoded_array, encoding);

	return 0;
}
