#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libpnm.h"
#include "generate_huffman_nodes.h"

#ifndef _HUFFMAN_ENCODE_IMAGE_
#define _HUFFMAN_ENCODE_IMAGE_

unsigned char *huffman_encode_image(struct PGM_Image *input_pgm_image, struct node *huffman_node, 
	int number_of_nodes, unsigned long int *length_of_encoded_array);

#endif