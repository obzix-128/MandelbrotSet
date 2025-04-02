#include <stdio.h>
#include <time.h>
#include <string.h>
#include <SFML/Graphics.hpp>

struct Fps_Data
{
    int frame_count;        // Счётчик кадров
    clock_t last_time;      // Время последнего обновления FPS
    double update_interval; // Интервал обновления (в секундах)
    int fps;                // Текущее значение FPS
};

struct Set_Data
{
    int WIDTH;    // Размеры комплексной плоскости, для которой выполняем подсчёт
    int HEIGHT;
    float DX;     // Единичные шаги по комплексной плоскости
    float DY;
    float DSCALE; // Единичный шаг увеличиния картинки
};

void calculateMandelbrot(const struct Set_Data* SET_INFO, long num_of_rep);
int  printMandelbrot    (const struct Set_Data* SET_INFO                 );
sf::Color calculateMandelbrotPixel(float actual_x, float actual_y);
int fps_counter(void);
