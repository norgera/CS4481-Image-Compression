#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "generate_huffman_nodes.h"
#include "store_huffman_encoded_data.h"

#ifndef _READ_HUFFMAN_ENCODED_DATA_
#define _READ_HUFFMAN_ENCODED_DATA_

unsigned char *read_huffman_encoded_data(char *compressed_file_name_ptr, int *image_width, 
	int *image_height, int *max_gray_value, int *number_of_nodes, struct node **huffman_node, 
	long int *length_of_encoded_image);

#endif