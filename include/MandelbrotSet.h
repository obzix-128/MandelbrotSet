#include <stdio.h>
#include <time.h>
#include <SFML/Graphics.hpp>

struct Fps_Data
{
    int frame_count;        // Счётчик кадров
    clock_t last_time;      // Время последнего обновления FPS
    double update_interval; // Интервал обновления (в секундах)
    int fps;                // Текущее значение FPS
};

sf::Color calculateMandelbrotPixel(float actual_x, float actual_y);
int fps_counter(void);
