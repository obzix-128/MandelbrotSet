#include "MandelbrotSet.h"


int main() 
{
    const int WIDTH  = 800; // Задали ширину экрана
    const int HEIGHT = 800; // Задали высоту экрана

    sf::Font font; // Создаем шрифт
    if (!font.openFromFile("georgia.ttf")) 
    {
        printf("Ошибка загрузки шрифта\n");
        return -1;
    }

    sf::Text text(font);                 // Создаем текстовый объект
    text.setCharacterSize(15);           // Задаём размер текста
    text.setFillColor(sf::Color::White); // Задаём цвет

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Mandelbrot Set"); // Создали окно

    while(window.isOpen()) // Пока окно открыто цикл повторяется
    {
        if(auto event = window.pollEvent()) // Проверяем события
        {
            if(event->is<sf::Event::Closed>()) // Если была ли команда закрыть окно (нажатие на крестик)
            {
                window.close();                // Если да, то закрываем окно
            }
        }
        
        sf::VertexArray pixels(sf::PrimitiveType::Points, WIDTH * HEIGHT); // Создаем массив пикселей

        for (int y = 0; y < HEIGHT; ++y)    // Начинаем перебор строк из пикселей
        {
            float dx = (16 / (float)WIDTH ); // Единичный шаг по оси x
            float dy = (16 / (float)HEIGHT); // Единичный шаг по оси y

            float actual_x = ((-WIDTH / 2) * dx);            // Актуальная координата пикселя в начале строки
            float actual_y = ((float)(y - HEIGHT / 2) * dy); // Актуальная координата отображаемой строки

            for (int x = 0; x < WIDTH; ++x) // Перебираем пиксели в строке
            {
                sf::Color color = calculateMandelbrotPixel(actual_x, actual_y); // Вычисляем цвет для пикселя 
                actual_x += dx;                                                 // Переходим на следующий пиксель

                pixels[y * WIDTH + x].position = sf::Vector2f((float)x, (float)y); // Устанавливаем позицию и цвет
                pixels[y * WIDTH + x].color    = color;
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
            return sf::Color(
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
    static FPS_Data fps_data = 
    {
        .frame_count = 0,
        .last_time = 0,
        .update_interval = 0.25, // Обновлять FPS каждые 0.5 секунды
        .fps = 0
    };

    clock_t current_time = clock();
    double elapsed = (double)(current_time - fps_data.last_time) / CLOCKS_PER_SEC;

    fps_data.frame_count++;

    if (elapsed >= fps_data.update_interval) // Если прошло больше 0.5 секунды — пересчитываем FPS
    {
        fps_data.fps = (int)(fps_data.frame_count / elapsed);
        fps_data.frame_count = 0;
        fps_data.last_time = current_time;
    }

    return fps_data.fps;
}