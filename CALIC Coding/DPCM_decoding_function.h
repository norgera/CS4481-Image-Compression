#ifndef DPCM_DECODING_FUNCTION_H
#define DPCM_DECODING_FUNCTION_H

#include "libpnm.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


void Decode_Using_DPCM (char *in_filename_Ptr);
void decodeTopWestRow(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int width);
void decodeSecondNorthRow(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int width);
void decodeFirstAndSecondNorthColumns(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int height);
void decodeLastNorthColumn(FILE *dpcmPointer, struct PGM_Image *image, int predArray[][image->width], int width, int height);

#endif // DPCM_DECODING_FUNCTION_H
