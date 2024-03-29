#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libpnm.h"

#ifndef _GENERATE_PIXEL_FREQUENCY_
#define _GENERATE_PIXEL_FREQUENCY_

long int *generate_pixel_frequency(struct PGM_Image *input_pgm_image, 
	int *number_of_non_zero_values_in_the_frequency_array);

#endif