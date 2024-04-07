#include "store_huffman_encoded_data.h"


void store_huffman_encoded_data(char *compressed_file_name_ptr, int image_width, 
	int image_height, int max_gray_value, int number_of_nodes, struct node *huffman_node, 
	long int length_of_encoded_image, unsigned char *encoded_image){

	FILE *f = fopen(compressed_file_name_ptr, "wb");

	// Writing header info
	struct header* file_header = malloc(sizeof(struct header));
	file_header->image_width = image_width;
	file_header->image_height = image_height;
	file_header->max_gray_value = max_gray_value;
	file_header->number_of_nodes = number_of_nodes;
	file_header->length_of_encoded_image = length_of_encoded_image;

	fwrite(file_header, sizeof(struct header), 1, f);

	// Writing the huffman nodes
	for(int i = 0; i < number_of_nodes; i++){
		fwrite(&huffman_node[i], sizeof(struct node), 1, f);
	}

	// Writing the encoded image
	fwrite(encoded_image, sizeof(unsigned char), length_of_encoded_image, f);

	fclose(f);

	free(file_header);
	free(huffman_node);
	free(encoded_image);
}




