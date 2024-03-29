#include "huffman_decode_image.h"

// Decodes the image using the huffman tree
struct PGM_Image *huffman_decode_image(int image_width, int image_height, 
	int max_gray_value, int number_of_nodes, struct node **huffman_node, 
	long int length_of_encoded_image, unsigned char *encoded_image){

    // Initialize the nodes
	struct tree_node* root;
	struct tree_node** all_nodes = malloc(sizeof(struct tree_node) * (max_gray_value + 1));
	for(int i = 0; i < max_gray_value + 1; i++){
		all_nodes[i] = malloc(sizeof(struct tree_node));
		all_nodes[i]->value = -1;
	}

    // Generate the huffman tree
	int left_val;
	int right_val;
	struct tree_node* left;
	struct tree_node* right;
	struct tree_node* linker;
	for(int i = 0; i < number_of_nodes; i++){

		// Get the left and right values
		left_val = huffman_node[i]->first_value;
		right_val = huffman_node[i]->second_value;

        // Get the left and right nodes
		left = all_nodes[left_val];
		right = all_nodes[right_val];

        // Set the values of the left and right nodes
		if(left->value == -1){
			left->value = left_val;
		}
		if(right->value == -1){
			right->value = right_val;
		}

        // Create a new node
		linker = malloc(sizeof(struct tree_node));
		linker->left = left;
		linker->right = right;
		linker->value = -2;

        // Set the linker node as the left and right nodes
		all_nodes[left_val] = linker;
		all_nodes[right_val] = linker;
	}
	root = linker; // Set the root of the tree


	struct PGM_Image* image = malloc(sizeof(struct PGM_Image)); // Initialize the image
	create_PGM_Image(image, image_width, image_height, max_gray_value); // Create the image

    // Initialize variables
	int pos_in_encoding = 0;
	int cur_encoding = encoded_image[pos_in_encoding++];
	int pos_in_encoding_bit = 8;
	int direction;

    // Decode the image
	for(int h = 0; h < image_height; h++){
		for(int w = 0; w < image_width; w++){
			struct tree_node* cur = root; // Set the current node as the root


			while(cur->value == -2){

				direction = cur_encoding & pos_in_encoding_bit;

                // if the direction is 0, go left, else go right
				if(direction == 0){
					cur = cur->left;
				} else {
					cur = cur->right;
				}

                // Shift the encoding bit
				pos_in_encoding_bit = pos_in_encoding_bit >> 1;

                // If the encoding bit is 0, get the next encoding
				if(pos_in_encoding_bit == 0){
					cur_encoding = encoded_image[pos_in_encoding++];
					pos_in_encoding_bit = 8;
				} 
			}

			image->image[h][w] = cur->value; // Set the pixel value
		}
	}
    // Free the nodes
	free(all_nodes);
	return image;
}
