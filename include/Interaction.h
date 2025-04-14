#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "MandelbrotSet.h"
#include <time.h>

enum ProgramMods
{
    NATIVE        = 1,
    FOR_OPT       = 2,
    INTRINSIC_OPT = 3
};

struct Fps_Data
{
    int frame_count;        // Счётчик кадров
    clock_t last_time;      // Время последнего обновления FPS
    double update_interval; // Интервал обновления (в секундах)
    int fps;                // Текущее значение FPS
};

ErrorNumbers processKeypresses(sf::RenderWindow& window, const struct Set_Data* SET_INFO, ProgramMods* mode, float* scale, 
                               float* offset_x, float* offset_y                                                           );
int fps_counter(void);

#endif // FPSCOUNTER_H