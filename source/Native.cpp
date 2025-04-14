#include "Native.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Заполняет массив pixels цветами пикселей. Без оптимизаций.
ARGUMENTS: sf::VertexArray& pixels - массив для заполнения цветами пикселей
           const struct Set_Data SET_INFO - информация о размере отображаемой плоскости, размере единичных шагов
           float scale    - множитель увеличения экрана
           float offset_x - смещение по оси x
           float offset_y - смещение по оси y
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNumbers getPixelColorsNative(sf::VertexArray& pixels, const struct Set_Data* SET_INFO, float scale, float offset_x, 
                                                                                                         float offset_y )
{
    CHECK_NULL_ADDR_ERROR(SET_INFO, NULL_ADDRESS_ERROR);

    for (int y = 0; y < SET_INFO->HEIGHT; ++y) // Начинаем перебор строк из пикселей
    {
        float actual_x = ((float)(    -SET_INFO->WIDTH / 2) * SET_INFO->DX) * scale + offset_x; 
        // Актуальная координата пикселя в начале строки
        float actual_y = ((float)(y - SET_INFO->HEIGHT / 2) * SET_INFO->DY) * scale + offset_y; 
        // Актуальная координата отображаемой строки

        for (int x = 0; x < SET_INFO->WIDTH; ++x) // Перебираем пиксели в строке
        {
            int counter = calculateMandelbrotPixelNative(actual_x, actual_y); // Вычисляем принадлежность к множеству Мандельброта
            actual_x += SET_INFO->DX * scale;                           // Переходим на следующий пиксель
            
            pixels[y * SET_INFO->WIDTH + x].position = sf::Vector2f((float)x, (float)y); // Устанавливаем позицию и цвет
            pixels[y * SET_INFO->WIDTH + x].color    = sf::Color
            (
                (uint8_t)(counter * 228) % 256,
                (uint8_t)(counter * 1488) % 256,
                (uint8_t)(counter * 1337) % 256                    
            );
        }
    }

    return NO_ERROR;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Функция, которая для каждой точки выполняет подсчёт, принадлежит ли она множеству Манделброта или нет. В зависимости от 
принадлежности, возвращает кол-во повторов, за которое точка покинула условный радиус, или ноль.
ARGUMENTS: float actual_x - координаты точки на экране по действительной оси
           float actual_y - координаты точки на экране по мнимой оси
------------------------------------------------------------------------------------------------------------------------------------*/
int calculateMandelbrotPixelNative(float actual_x, float actual_y) 
{
    float x_coordinate = actual_x; // Координаты точки P_0
    float y_coordinate = actual_y;

    const int MAX_ITERATIONS = 256; // Число повторений, необходимое, чтобы считать точку принадлежащей множеству
    const float SQUARE_MAX_RADIUS = 100; // Если в ходе подсчётов точка покинет этот радиус, то считаем её не принадлежащей множеству

    for(int counter = 0; counter < MAX_ITERATIONS; ++counter) // Цикл для вычисления принадлежности точки к множеству
    {
        float square_x_coordinate = x_coordinate * x_coordinate;
        float square_y_coordinate = y_coordinate * y_coordinate;
        float xy = x_coordinate * y_coordinate; 
        float square_radius = square_x_coordinate + square_y_coordinate;

        if(square_radius >= SQUARE_MAX_RADIUS) // Вернём кол-во повторений, за которое точка покинула радиус
        {
            return counter;
        }

        x_coordinate = square_x_coordinate - square_y_coordinate + actual_x; // Координаты точки для следующей итерации
        y_coordinate = xy + xy + actual_y;
    }

    return 0; // Если точка принадлежит множеству, вернём 0
}

