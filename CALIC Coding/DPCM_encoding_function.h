#ifndef DPCM_ENCODING_FUNCTION_H
#define DPCM_ENCODING_FUNCTION_H

#include "libpnm.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


void Encode_Using_DPCM(char *in_PGM_filename_Ptr, int prediction_rule, float *avg_abs_error_Ptr, float *std_abs_error_Ptr);
void encodeTopWestRow(FILE *filePointer, struct PGM_Image *image, int width);
void encodeSecondNorthRow(FILE *filePointer, struct PGM_Image *image, int width);
void encodeFirstAndSecondNorthColumns(FILE *filePointer, struct PGM_Image *image, int height);
void encodeLastNorthColumn(FILE *filePointer, struct PGM_Image *image, int width, int height);

#endif // DPCM_ENCODING_FUNCTION_H  
