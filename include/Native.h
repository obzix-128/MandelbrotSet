#ifndef NATIVE_H
#define NATIVE_H

#include "MandelbrotSet.h"

ErrorNumbers getPixelColorsNative(sf::VertexArray& pixels, const struct Set_Data* SET_INFO, 
                                  const struct Const_For_Colors* COLORS_INFO, float scale, float offset_x, float offset_y);
int calculateMandelbrotPixelNative(float actual_x, float actual_y);

#endif // NATIVE_H
