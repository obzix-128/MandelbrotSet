#ifndef INTRINSICOPT_H
#define INTRINSICOPT_H

#include "MandelbrotSet.h"
#include <immintrin.h>

ErrorNumbers getPixelColorsIntrinsicOpt(sf::VertexArray& pixels, const struct Set_Data* SET_INFO, float scale, float offset_x, 
                                                                                                               float offset_y );
__m256i calculateMandelbrotPixelIntrinsicOpt(__m256 x_coord, __m256 y_coord);

#endif // INTRINSICOPT_H