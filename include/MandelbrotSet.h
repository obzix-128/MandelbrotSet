#ifndef MANDELBROTSET_H
#define MANDELBROTSET_H

#include <stdio.h>
#include <string.h>
#include <SFML/Graphics.hpp>

enum ErrorNumbers
{
    NO_ERROR            = 0,
    LOADING_FONT_ERROR  = 1,
    NULL_ADDRESS_ERROR  = 2,
    MODE_ERROR          = 3
};

#define CHECK_NULL_ADDR_ERROR(variable, ERROR_NUMBER) if(variable == NULL)     \
                                                      {                        \
                                                          return ERROR_NUMBER; \
                                                      }

struct __attribute__((aligned(32))) Set_Data
{
    int WIDTH;    // Размеры комплексной плоскости, для которой выполняем подсчёт
    int HEIGHT;
    float DX;     // Единичные шаги по комплексной плоскости
    float DY;
    float DSCALE; // Единичный шаг увеличиния картинки
};

#endif // MANDELBROTSET_H