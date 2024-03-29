#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libpnm.h"
#include "generate_huffman_nodes.h"

#ifndef _HUFFMAN_DECODE_IMAGE_
#define _HUFFMAN_DECODE_IMAGE_

struct tree_node{
	struct tree_node* left;
	struct tree_node* right;
	int value;
};

struct PGM_Image *huffman_decode_image(int image_width, int image_height, 
	int max_gray_value, int number_of_nodes, struct node **huffman_node, 
	long int length_of_encoded_image, unsigned char *encoded_image);

#endif

