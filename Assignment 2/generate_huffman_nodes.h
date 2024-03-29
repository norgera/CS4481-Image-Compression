#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "libpnm.h"

#ifndef _GENERATE_HUFFMAN_NODES_
#define _GENERATE_HUFFMAN_NODES_

struct node {
	int first_value; 
	int second_value;
};

struct node *generate_huffman_nodes(long int *pixel_frequency, int max_gray_value, 
	int number_of_non_zero_values_in_the_frequency_array);

#endif