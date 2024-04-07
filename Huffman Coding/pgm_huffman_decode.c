#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libpnm.h"

#include "generate_huffman_nodes.h"
#include "store_huffman_encoded_data.h"
#include "huffman_decode_image.h"
#include "read_huffman_encoded_data.h"

int main(int argc, char *argv[]){

	// Checking args
	if(argc != 3){
		printf("Please ensure you have entered two arguments. You entered: %d\n", argc - 1);
		exit(0);
	}

	if(access(argv[1], F_OK) == -1){
		printf("Please ensure you have entered a valid compressed file.\n");
		exit(0);
	}

	unsigned char* encoded_image;
	struct header* file_header = malloc(sizeof(struct header));
	struct node** huffman_node = malloc(sizeof(struct node**) * 100000);

	// Loading in from the input file
	encoded_image = read_huffman_encoded_data(argv[1], &(file_header->image_width), &(file_header->image_height), 
		&(file_header->max_gray_value), &(file_header->number_of_nodes), huffman_node, 
		&(file_header->length_of_encoded_image));
	
	// Decoding the image
	struct PGM_Image* image;
	image = huffman_decode_image(file_header->image_width, file_header->image_height, 
		file_header->max_gray_value, file_header->number_of_nodes, huffman_node, 
		file_header->length_of_encoded_image, encoded_image);
	
	// Save the image
	save_PGM_Image(image, argv[2], 1);
	

	return 0;
}