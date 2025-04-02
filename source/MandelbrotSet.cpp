#include "MandelbrotSet.h"


int main(const int argc, const char** argv) 
{        
    const int _MIN_NUMBERS_OF_ARGUMENTS = 2;
    if(argc < _MIN_NUMBERS_OF_ARGUMENTS) // На случай, если кто-то забыл задать аргументы в командной строке
    {
        fprintf(stderr, "Ошибка: не заданы аругменты\n");
        return -1;
    }

    const struct Set_Data SET_INFO = 
    {
        .WIDTH  = 1000,
        .HEIGHT = 1000,
        .DX = 1.f / 1000, 
        .DY = 1.f / 1000,
        .DSCALE = 0.1f
    };

    if(0 == strcmp(argv[1], "standart")) // Стандартный запуск программы, рисуем множество Мандельброта
    {
        printMandelbrot(&SET_INFO);   
    }
    else if(0 == strcmp(argv[1], "test")) // Запуск программы для снятия измерений
    {
        char* endptr = NULL;
        long num_of_rep = strtol(argv[2], &endptr, 10); // Преобразуем второй аргумент командной строки в число
        if(argv[2] == endptr)                           // Если преобразования не произошло, выводим ошибку
        {
            fprintf(stderr, "Ошибка: не обнаружено число повторений\n");
            return -1;
        }
        calculateMandelbrot(&SET_INFO, num_of_rep);
        return 0;
    }
    else
    {
        fprintf(stderr, "Ошибка: Неверно заданы аргументы\n");
        return -1;
    }

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Функция, которая проверяет для каждой точки плоскости указанных размеров, принадлежит та множеству Мандельброта или нет
ARGUMENTS: const struct Set_Data SET_INFO - информация о размере плоскости, размере единичных шагов
           long num_of_rep - количество дополнительных пересчётов, для увеличения нагрузки
------------------------------------------------------------------------------------------------------------------------------------*/
void calculateMandelbrot(const struct Set_Data* SET_INFO, long num_of_rep)
{
    while(true)
    {
        for(long i = 0; i < num_of_rep; ++i) // Задаём число дополнительных пересчётов (для дополнительной нагрузки)
        {
            for (int y = 0; y < SET_INFO->HEIGHT; ++y) // Начинаем перебор строк из пикселей
            {
                float actual_x = (float)(    -SET_INFO->WIDTH / 2); 
                // Актуальная координата пикселя в начале строки
                float actual_y = (float)(y - SET_INFO->HEIGHT / 2); 
                // Актуальная координата отображаемой строки

                for (int x = 0; x < SET_INFO->WIDTH; ++x) // Перебираем пиксели в строке
                {
                    calculateMandelbrotPixel(actual_x, actual_y); // Вычисляем цвет для пикселя 
                    actual_x += SET_INFO->DX;                     // Переходим на следующий пиксель
                }
            }
        }
        int fps = fps_counter();
        fprintf(stdout, "fps = %d\n", fps);
    }
}

/*------------------------------------------------------------------------------------------------------------------------------------
Функция, которая рисует в окне множество Мандельброта, с возможностью перемещения и увелечения картинки. Каждому пикселю соответствует
точка комплексной плоскости, для которой выполняется проверка на принадлежность множеству Мандельброта
ARGUMENTS: const struct Set_Data SET_INFO - информация о размере отображаемой плоскости, размере единичных шагов
------------------------------------------------------------------------------------------------------------------------------------*/
int printMandelbrot(const struct Set_Data* SET_INFO)
{
    sf::Font font; // Создаем шрифт
    if (!font.openFromFile("georgia.ttf")) 
    {
        printf("Ошибка загрузки шрифта\n");
        return -1;
    }

    sf::Text text(font);                 // Создаем текстовый объект
    text.setCharacterSize(15);           // Задаём размер текста
    text.setFillColor(sf::Color::White); // Задаём цвет

    sf::RenderWindow window(sf::VideoMode({(unsigned)SET_INFO->WIDTH, (unsigned)SET_INFO->HEIGHT}), "Mandelbrot Set"); // Создали окно
    
    sf::VertexArray pixels(sf::PrimitiveType::Points, SET_INFO->WIDTH * SET_INFO->HEIGHT); // Создаем массив пикселей

    float scale = 1;    // Увеличение
    float offset_x = 0; // Смещение по оси x
    float offset_y = 0; // Смещение по оси y 

    while(window.isOpen()) // Пока окно открыто цикл повторяется
    {
        if(auto event = window.pollEvent()) // Проверяем события
        {
            if(event->is<sf::Event::Closed>()) // Если была ли команда закрыть окно (нажатие на крестик)
            {
                window.close();                // Если да, то закрываем окно
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q)) // Горячая клавиша для закрытия окна
        {
            window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Z)) // Приблизить
        {
            scale -= SET_INFO->DSCALE;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::X)) // Отдалить
        {
            scale += SET_INFO->DSCALE;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) // Влево
        {
            offset_x -= SET_INFO->DX * 50;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) // Вправо
        {
            offset_x += SET_INFO->DX * 50;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) // Вверх
        {
            offset_y -= SET_INFO->DY * 50;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) // Вниз
        {
            offset_y += SET_INFO->DY * 50;
        }

        for (int y = 0; y < SET_INFO->HEIGHT; ++y) // Начинаем перебор строк из пикселей
        {
            float actual_x = (((float)(    -SET_INFO->WIDTH / 2) * SET_INFO->DX) + offset_x) * scale; 
            // Актуальная координата пикселя в начале строки
            float actual_y = (((float)(y - SET_INFO->HEIGHT / 2) * SET_INFO->DY) + offset_y) * scale; 
            // Актуальная координата отображаемой строки

            for (int x = 0; x < SET_INFO->WIDTH; ++x) // Перебираем пиксели в строке
            {
                sf::Color color = calculateMandelbrotPixel(actual_x, actual_y); // Вычисляем цвет для пикселя 
                actual_x += SET_INFO->DX * scale;                               // Переходим на следующий пиксель

                pixels[y * SET_INFO->WIDTH + x].position = sf::Vector2f((float)x, (float)y); // Устанавливаем позицию и цвет
                pixels[y * SET_INFO->WIDTH + x].color    = color;
            }
        }

        text.setString("fps " + std::to_string(fps_counter())); // Устанавливаем строку текста

        window.clear();      // Очищаем экран
        window.draw(pixels); // Отрисовываем все пиксели одним вызовом
        window.draw(text);   // Отображаем кол-во fps
        window.display();    // Отображает нарисованные пиксели на экран
    }

    return 0;
}

/*------------------------------------------------------------------------------------------------------------------------------------
Функция, которая для каждой точки выполняет подсчёт, принадлежит ли она множеству Манделброта или нет. В зависимости от принадлежности
и скорости выхода координат точки за пределы некторой окрестности (для не принадлежащих множеству точек), возвращает цвет для
отображения этой точки на экране
ARGUMENTS: float actual_x - координаты точки на экране по действительной оси
           float actual_y - координаты точки на экране по мнимой оси
------------------------------------------------------------------------------------------------------------------------------------*/
sf::Color calculateMandelbrotPixel(float actual_x, float actual_y) 
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

        if(square_radius >= SQUARE_MAX_RADIUS) // Если вышли за SQUARE_MAX_RADIUS, покрасим точку в цвет, зависящий от counter
        {
            return sf::Color
            (
                (uint8_t)(counter * 228 ) % 256,       
                (uint8_t)(counter * 1488) % 256,       
                (uint8_t)(counter * 1337) % 256  
            );
        }

        x_coordinate = square_x_coordinate - square_y_coordinate + actual_x; // Координаты точки для следующей итерации
        y_coordinate = xy + xy + actual_y;
    }

    return sf::Color::Black; // Если точка принадлежит множеству, обозначим её чёрным цветом
}

/*------------------------------------------------------------------------------------------------------------------------------------
Вычисляет среднее значение fps за последние 0,25 секунд и возвращает в виде числа int
ARGUMENTS: None
------------------------------------------------------------------------------------------------------------------------------------*/
int fps_counter() 
{
    static Fps_Data fps_data = 
    {
        .frame_count = 0,
        .last_time = 0,
        .update_interval = 0.25, // Обновлять FPS каждые 0.25 секунды
        .fps = 0
    };

    clock_t current_time = clock();
    double elapsed = (double)(current_time - fps_data.last_time) / CLOCKS_PER_SEC;

    fps_data.frame_count++;

    if (elapsed >= fps_data.update_interval) // Если прошло больше 0.25 секунды — пересчитываем FPS
    {
        fps_data.fps = (int)(fps_data.frame_count / elapsed);
        fps_data.frame_count = 0;
        fps_data.last_time = current_time;
    }

    return fps_data.fps;
}