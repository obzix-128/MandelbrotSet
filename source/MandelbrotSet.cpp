#include "MandelbrotSet.h"
#include "ErrorHandler.h"
#include "Interaction.h"
#include "Native.h"
#include "ForOpt.h"
#include "IntrinsicOpt.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Рисует в окне множество Мандельброта, с возможностью перемещения и увелечения картинки. Каждому пикселю соответствует
точка комплексной плоскости, для которой выполняется проверка на принадлежность множеству Мандельброта
------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) 
{        
    #define WIDTH_D  800
    #define HEIGHT_D 800

    const struct Set_Data SET_INFO = // Информация для отображения картинки.
    {
        .WIDTH  = WIDTH_D,    // Ширина окна
        .HEIGHT = HEIGHT_D,   // Высота окна
        .DX = 1.f / WIDTH_D,  // Единичный шаг между пикселями по горизонтали
        .DY = 1.f / HEIGHT_D, // Единичный шаг между пикселями по вертикали
        .DSCALE = 1.5f        // Множитель увеличения
    };

    if((WIDTH_D * HEIGHT_D) % 8 != 0)
    {
        errorHandler(LOADING_FONT_ERROR, __PRETTY_FUNCTION__);
        return LOADING_FONT_ERROR;
    }

    #undef WIDTH_D
    #undef HEIGHT_D

    const struct Const_For_Colors COLORS_INFO = // Константы, от которых зависит раскраска множества Мандельброта
    {
        .RED   = 2000,
        .GREEN = 707,
        .BLUE  = 2017
    };

    sf::Font font; // Создаем шрифт
    if (!font.openFromFile("georgia.ttf")) 
    {
        errorHandler(LOADING_FONT_ERROR, __PRETTY_FUNCTION__);
        return LOADING_FONT_ERROR;
    }

    sf::Text text(font);                 // Создаем текстовый объект
    text.setCharacterSize(15);           // Задаём размер текста
    text.setFillColor(sf::Color::White); // Задаём цвет

    sf::RenderWindow window(sf::VideoMode({(unsigned)SET_INFO.WIDTH, (unsigned)SET_INFO.HEIGHT}), "Mandelbrot Set"); // Создали окно
    
    sf::VertexArray pixels(sf::PrimitiveType::Points, SET_INFO.WIDTH * SET_INFO.HEIGHT); // Создаем массив пикселей

    float scale    = 4; // Увеличение
    float offset_x = 0; // Смещение по оси x
    float offset_y = 0; // Смещение по оси y 
    ErrorNumbers check_error = NO_ERROR;
    ProgramMods mode = NATIVE; // Режим работы программы (изначально без искусственных оптимизаций)

    while(window.isOpen()) // Пока окно открыто цикл повторяется
    {
        CHECK_ERROR(processKeypresses(window, &SET_INFO, &mode, &scale, &offset_x, &offset_y)); // Обрабатываем нажатия клавиш

        if(mode == NATIVE) // Режим без ручных оптимизаций
        {
            CHECK_ERROR(getPixelColorsNative(pixels, &SET_INFO, &COLORS_INFO, scale, offset_x, offset_y));
        }
        else if(mode == FOR_OPT) // Режим с оптимизацией с помощью циклов
        {
            CHECK_ERROR(getPixelColorsForOpt(pixels, &SET_INFO, &COLORS_INFO, scale, offset_x, offset_y));
        }
        else if(mode == INTRINSIC_OPT) // Режим с оптимизацией с помощью intrinsic-ов
        {
            CHECK_ERROR(getPixelColorsIntrinsicOpt(pixels, &SET_INFO, &COLORS_INFO, scale, offset_x, offset_y));
        }
        else // Ловим ошибку
        {
            errorHandler(MODE_ERROR, __PRETTY_FUNCTION__);
            return MODE_ERROR;
        }

        text.setString("fps " + std::to_string(fps_counter())); // Устанавливаем строку текста

        window.clear();      // Очищаем экран
        window.draw(pixels); // Отрисовываем все пиксели одним вызовом
        window.draw(text);   // Отображаем кол-во fps
        window.display();    // Отображает нарисованные пиксели на экран
    }

    return 0;
}
