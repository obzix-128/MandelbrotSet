#ifndef FOROPT_H
#define FOROPT_H

#include "ErrorHandler.h"

#define BLOCK_SIZE 8 // Размер одного блока пикселей для оптимизацией циклами

ErrorNumbers getPixelColorsForOpt(sf::VertexArray& pixels, const struct Set_Data* SET_INFO, 
                                  const struct Const_For_Colors* COLORS_INFO, float scale, float offset_x, float offset_y);
void calculateMandelbrotPixelForOpt(float* x_coords, float y_coord, int* counters);

#endif // FOROPT_H