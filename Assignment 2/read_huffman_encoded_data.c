#include "read_huffman_encoded_data.h"

unsigned char *read_huffman_encoded_data(char *compressed_file_name_ptr, int *image_width, 
	int *image_height, int *max_gray_value, int *number_of_nodes, struct node **huffman_node, 
	long int *length_of_encoded_image){

	struct header* file_header = malloc(sizeof(struct header));

	FILE *f = fopen(compressed_file_name_ptr, "rb");
	fread(file_header, sizeof(struct header), 1, f);

	// Getting the information from the header
	*image_width = file_header->image_width;
	*image_height = file_header->image_height;
	*max_gray_value = file_header->max_gray_value;
	*number_of_nodes = file_header->number_of_nodes;
	*length_of_encoded_image = file_header->length_of_encoded_image;

	// Getting all of the huffman nodes
	*huffman_node = malloc(sizeof(struct node) * (*number_of_nodes));
	for(int i = 0; i < *number_of_nodes; i++){
		huffman_node[i] = malloc(sizeof(struct node));
		fread(huffman_node[i], sizeof(struct node), 1, f);
		// printf("%d / %d : %d %d\n", i, *number_of_nodes, huffman_node[i]->first_value, huffman_node[i]->second_value);
	}

	// Read in encoded text
	unsigned char* encoded_image = malloc(sizeof(unsigned char) * (*length_of_encoded_image));
	fread(encoded_image, sizeof(unsigned char), *length_of_encoded_image, f);

	fclose(f);
	return encoded_image;
}