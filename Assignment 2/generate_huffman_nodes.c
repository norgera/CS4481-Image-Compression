#include "generate_huffman_nodes.h"

// returns the maximum of two integers
int max(int x, int y){
	if(x > y){
		return x;
	}
	return y;
}

// returns the minimum of two integers
int min(int x, int y){
	if(x < y){
		return x;
	}
	return y;
}

// generates the huffman nodes
struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value,
	int number_of_non_zero_values_in_the_frequency_array){

	struct node* nodes = malloc(sizeof(struct node) * number_of_non_zero_values_in_the_frequency_array - 1);

    // Initialize the nodes
	long cur_min1;
	long cur_min2;
	int cur_pos1;
	int cur_pos2;
	struct node* n;

    // Generate the huffman nodes
	for(int i = 0; i < number_of_non_zero_values_in_the_frequency_array; i++){

		cur_min1 = LONG_MAX;
		cur_min2 = LONG_MAX;

        // Find the two smallest values
		for(int p = 0; p < MAX_GRAY_VALUE + 1; p++){
			if(pixel_frequency[p] != 0){
				if(pixel_frequency[p] < cur_min1){

					cur_min2 = cur_min1;
					cur_pos2 = cur_pos1;

					cur_min1 = pixel_frequency[p];
					cur_pos1 = p;

                    // If the current value is smaller than the second smallest value
				} else if (pixel_frequency[p] < cur_min2){

					cur_min2 = pixel_frequency[p];
					cur_pos2 = p;

				}
			}
		}

        // Combine the two smallest values
		pixel_frequency[cur_pos1] += pixel_frequency[cur_pos2];
		pixel_frequency[cur_pos2] = LONG_MAX;

        // Create a new node
		n = malloc(sizeof(struct node));
		n->first_value = min(cur_pos1, cur_pos2);
		n->second_value = max(cur_pos1, cur_pos2);
		nodes[i] = *n;

	}

    // Free the pixel frequency array
	free(pixel_frequency);
	return nodes;
}
