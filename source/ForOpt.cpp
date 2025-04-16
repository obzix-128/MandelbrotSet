#include "ForOpt.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Заполняет массив pixels цветами пикселей. Оптимизация через обработку блоков по BLOCK_SIZE пикселей
ARGUMENTS: sf::VertexArray& pixels - массив для заполнения цветами пикселей
           const struct Set_Data SET_INFO - информация о размере отображаемой плоскости, размере единичных шагов
           float scale    - множитель увеличения экрана
           float offset_x - смещение по оси x
           float offset_y - смещение по оси y
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNumbers getPixelColorsForOpt(sf::VertexArray& pixels, const struct Set_Data* SET_INFO, 
                                  const struct Const_For_Colors* COLORS_INFO, float scale, float offset_x, float offset_y)
{
    CHECK_NULL_ADDR_ERROR(SET_INFO, NULL_ADDRESS_ERROR);

    for(int y = 0; y < SET_INFO->HEIGHT; ++y) // Начинаем перебор строк из пикселей
    {
        const float actual_y = ((float)(y - SET_INFO->HEIGHT / 2) * SET_INFO->DY) * scale + offset_y; 
        // Актуальная координата отображаемой строки        

        float x_coords[BLOCK_SIZE] = {}; // Массив для хранения BLOCK_SIZE значений x

        for(int i = 0; i < BLOCK_SIZE; ++i) // Координаты по горизонтали первых восьми пикселей в строке
        {
            x_coords[i] = (float)(i - SET_INFO->WIDTH / 2) * SET_INFO->DX * scale + offset_x;
        }

        const float dx = SET_INFO->DX * BLOCK_SIZE * scale; // Шаг для следующего блока из BLOCK_SIZE точек

        for(int x = 0; x < SET_INFO->WIDTH; x += BLOCK_SIZE) // Обрабатываем по BLOCK_SIZE пикселей за итерацию
        {
            int counter_values[BLOCK_SIZE] = {}; // Задаём счётчик для определения принадлежности множеству Мандельброта
            calculateMandelbrotPixelForOpt(x_coords, actual_y, counter_values); // Считаем принадлежность множеству

            for(int i = 0; i < BLOCK_SIZE; ++i) // Задаём цвет пикселей из блока
            {
                pixels[y * SET_INFO->WIDTH + x + i].position = sf::Vector2f((float)(x + i), (float)y);
                pixels[y * SET_INFO->WIDTH + x + i].color = sf::Color
                (
                    (uint8_t)(counter_values[i] * COLORS_INFO->RED)   % 256,
                    (uint8_t)(counter_values[i] * COLORS_INFO->GREEN) % 256,
                    (uint8_t)(counter_values[i] * COLORS_INFO->BLUE)  % 256
                );  
            }

            for (int i = 0; i < BLOCK_SIZE; ++i) // Обновляем координаты x для следующего блока
            {
                x_coords[i] += dx;
            }
        }
    }

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Функция, которая для каждой точки выполняет подсчёт, принадлежит ли она множеству Манделброта или нет.
Обрабатывает BLOCK_SIZE точек одновременно через массивы.
ARGUMENTS: float x_coords - координаты BLOCK_SIZE точек по действительной оси
           float y_coord - координата по мнимой оси (одинакова для всех BLOCK_SIZE точек)
           int counters - массив для записи результатов (количество итераций)
------------------------------------------------------------------------------------------------------------------------------------*/
void calculateMandelbrotPixelForOpt(float* x_coords, float y_coord, int* counters)
{
    const int MAX_ITERATIONS = 256;         // Число повторений, необходимое, чтобы считать точку принадлежащей множеству
    const float SQUARE_MAX_RADIUS = 100.0f; // Если точка покинет этот радиус, то считаем её не принадлежащей множеству
    
    float x[BLOCK_SIZE] = {};
    float y[BLOCK_SIZE] = {};
    bool active[BLOCK_SIZE] = {};
    
    for(int i = 0; i < BLOCK_SIZE; ++i) // Задаём начальные координаты точек для подсчётов
    {
        x[i] = x_coords[i];
        y[i] = y_coord;
        counters[i] = 0;
        active[i] = true;
    }

    for(int iter = 0; iter < MAX_ITERATIONS; ++iter) // Считаем принадлежность множеству Мандельброта
    {
        bool any_active = false; // Если все точки покинули SQUARE_MAX_RADIUS, то по этому флагу прервём цикл
        
        for(int i = 0; i < BLOCK_SIZE; ++i) 
        {
            if(!active[i]) // Пропускаем точки, поикнувшие SQUARE_MAX_RADIUS 
            {
                continue;
            }

            float x2 = x[i] * x[i];
            float y2 = y[i] * y[i];
            float r2 = x2 + y2;
            
            if(r2 < SQUARE_MAX_RADIUS) 
            {
                counters[i]++;
                any_active = true;
                
                float xy = x[i] * y[i];
                x[i] = x2 - y2 + x_coords[i];
                y[i] = xy + xy + y_coord;
            } 
            else 
            {
                active[i] = false;
            }
        }
        
        if(!any_active) // Если все точки покинули SQUARE_MAX_RADIUS, то покидаем цикл
        {
            break;
        }
    }
}