#include "Interaction.h"


/*------------------------------------------------------------------------------------------------------------------------------------
Обрабатывает нажатия клавиш.
ARGUMENTS: sf::RenderWindow& window - окно, в котором мы рисуем множество Мандельброта
           const struct Set_Data SET_INFO - информация о размере отображаемой плоскости, размере единичных шагов
           float* scale    - множитель увеличения экрана
           float* offset_x - смещение по оси x
           float* offset_y - смещение по оси y
------------------------------------------------------------------------------------------------------------------------------------*/
ErrorNumbers processKeypresses(sf::RenderWindow& window, const struct Set_Data* SET_INFO, ProgramMods* mode, float* scale, 
                               float* offset_x, float* offset_y                                                           )
{
    CHECK_NULL_ADDR_ERROR(SET_INFO, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(scale,    NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(offset_x, NULL_ADDRESS_ERROR);
    CHECK_NULL_ADDR_ERROR(offset_y, NULL_ADDRESS_ERROR);

    while (const std::optional event = window.pollEvent())
    {
        if(event->is<sf::Event::Closed>() || // Проверка на команду закрыть окно (нажатие на крестик) или "Q"
           (event->is<sf::Event::KeyPressed>() &&
            event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Q)) 
        {
            window.close(); // Если да, то закрываем окно
        }

        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Num1) // Режим без оптимизации
        {
            *mode = NATIVE;
        }
        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Num2) // Режим без оптимизации
        {
            *mode = FOR_OPT;
        }
        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Num3) // Режим без оптимизации
        {
            *mode = INTRINSIC_OPT;
        }

        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Z) // Приблизить
        {
            *scale /= SET_INFO->DSCALE;
        }
        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::X) // Отдалить
        {
            *scale *= SET_INFO->DSCALE;
        }

        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::A) // Влево
        {
            *offset_x -= SET_INFO->DX * 50 * *scale;
        }
        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D) // Вправо
        {
            *offset_x += SET_INFO->DX * 50 * *scale;
        }
        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::W) // Вверх
        {
            *offset_y -= SET_INFO->DY * 50 * *scale;
        }
        if(event->is<sf::Event::KeyPressed>() &&
           event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S) // Вниз
        {
            *offset_y += SET_INFO->DY * 50 * *scale;
        }
    }

    return NO_ERROR;
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