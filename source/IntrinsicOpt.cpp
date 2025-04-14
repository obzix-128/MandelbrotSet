#include "IntrinsicOpt.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Заполняет массив pixels цветами пикселей. Для оптимизации использую intrinsic-и
ARGUMENTS: sf::VertexArray& pixels - массив для заполнения цветами пикселей
           const struct Set_Data SET_INFO - информация о размере отображаемой плоскости, размере единичных шагов
           float scale    - множитель увеличения экрана
           float offset_x - смещение по оси x
           float offset_y - смещение по оси y
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNumbers getPixelColorsIntrinsicOpt(sf::VertexArray& pixels, const struct Set_Data* SET_INFO, float scale, float offset_x, 
                                                                                                               float offset_y )
{
    CHECK_NULL_ADDR_ERROR(SET_INFO, NULL_ADDRESS_ERROR);

    const int FLOATS_IN_MM256 = 8; // Именно 8 значений типа float помещается в 256-битном регистре

    for (int y = 0; y < SET_INFO->HEIGHT; ++y) // Начинаем перебор строк из пикселей
    {
        const float actual_y = ((float)(y - SET_INFO->HEIGHT / 2) * SET_INFO->DY) * scale  + offset_y;
        const __m256 y_coord = _mm256_set1_ps(actual_y); // Актуальная координата y для блока из 8 точек по x

        const __m256 dx = _mm256_set1_ps(SET_INFO->DX * FLOATS_IN_MM256 * scale); // Переменная для работы сразу с 8 значениями по x
        
        __m256 x_coord = _mm256_setr_ps // Начальное значение x для блока из 8 пикселей
        (
            ((float)(0 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(1 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(2 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(3 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(4 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(5 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(6 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x,
            ((float)(7 - SET_INFO->WIDTH / 2) * SET_INFO->DX)* scale + offset_x
        );

        for (int x = 0; x < SET_INFO->WIDTH; x += FLOATS_IN_MM256) // Обрабатываем по 8 пикселей за итерацию
        {
            __m256i counters = calculateMandelbrotPixelIntrinsicOpt(x_coord, y_coord); // Вычисляем 8 пикселей одновременно
            x_coord = _mm256_add_ps(x_coord, dx);                                      // Переходим к следующему блоку из 8 пикселей

            alignas(32) int counter_values[FLOATS_IN_MM256];
            // Переводим значение 256-битного регистра в массив из 8 int-ов
            _mm256_store_si256((__m256i*)counter_values, counters); 

            for (int i = 0; i < FLOATS_IN_MM256; ++i) //Задаём цвет пикселей из блока
            {
                pixels[y * SET_INFO->WIDTH + x + i].position = sf::Vector2f((float)(x + i), (float)y); 
                pixels[y * SET_INFO->WIDTH + x + i].color = sf::Color // Устанавливаем позицию и цвет
                (
                    (uint8_t)(counter_values[i] * 228) % 256,
                    (uint8_t)(counter_values[i] * 1488) % 256,
                    (uint8_t)(counter_values[i] * 1337) % 256
                );
            }
        }
    }

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Функция, которая для каждой точки выполняет подсчёт, принадлежит ли она множеству Манделброта или нет. В зависимости от принадлежности
и скорости выхода координат точки за пределы некторой окрестности (для не принадлежащих множеству точек), возвращает цвет для
отображения этой точки на экране
ARGUMENTS: float x_coord - координаты точки на экране по действительной оси
           float y_coord - координаты точки на экране по мнимой оси
------------------------------------------------------------------------------------------------------------------------------------*/
__m256i calculateMandelbrotPixelIntrinsicOpt(__m256 x_coord, __m256 y_coord)
{
    const int MAX_ITERATIONS = 256; // Число повторений, необходимое, чтобы считать точку принадлежащей множеству
    const __m256 SQUARE_MAX_RADIUS = _mm256_set1_ps(100.0f); // Блок из 8 значений максимального радиуса
    
    __m256i counters = _mm256_setzero_si256();
    __m256 x = x_coord;
    __m256 y = y_coord;
    
    const __m256i ones = _mm256_set1_epi32(1);

    for (int i = 0; i < MAX_ITERATIONS; ++i)
    {
        __m256 x2 = _mm256_mul_ps(x, x);
        __m256 y2 = _mm256_mul_ps(y, y);
        __m256 xy = _mm256_mul_ps(x, y);
        __m256 r2 = _mm256_add_ps(x2, y2);
        
        __m256 new_mask = _mm256_cmp_ps(r2, SQUARE_MAX_RADIUS, _CMP_LT_OQ); // Проверяем условие r2 < 100
        
        if (_mm256_testz_ps(new_mask, new_mask) == 1) // Если все точки вышли за радиус, прерываем цикл
        {
            break;
        }
            
        // Обновляем счетчики только для активных точек
        counters = _mm256_add_epi32(counters, _mm256_and_si256(_mm256_castps_si256(new_mask), ones));
        
        x = _mm256_add_ps(_mm256_sub_ps(x2, y2), x_coord); // Вычисляем новые координаты
        y = _mm256_add_ps(_mm256_add_ps(xy, xy), y_coord);
        }
    
    return counters;
}
