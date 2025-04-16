# Mandelbrot Set
<img src="https://github.com/user-attachments/assets/9c81d36c-0835-463d-b92d-098ecb10fdc2" alt="Mandelbrot Set" width="400" height="400">

## Суть проекта
Основная идея этого проекта - на примере построения [**множества Мандельброта**](https://ru.wikipedia.org/wiki/%D0%9C%D0%BD%D0%BE%D0%B6%D0%B5%D1%81%D1%82%D0%B2%D0%BE_%D0%9C%D0%B0%D0%BD%D0%B4%D0%B5%D0%BB%D1%8C%D0%B1%D1%80%D0%BE%D1%82%D0%B0) изучить различные методы оптимизации: аппаратно-ориентированную и автоматическую оптимизацию компилятора. 

## Математическое определение множества Мандельброта

Множество Мандельброта ℳ определяется как множество комплексных чисел c ∈ ℂ, для которых рекуррентная последовательность: <br>

> $Z_n = Z_{n-1}^2 + Z_0$ <br>

остается ограниченной при n → ∞.
$Z_0$ - фиксированная точка на комплексной плоскости.

 Выражение в координатах выглядит следующим образом: <br>
> $x_{n+1} = x_n^2 - y_n^2 + x_0$ <br>
$y_{n+1} = 2x_ny_n ~~~  + y_0$

### Критерий принадлежности
Точка c принадлежит ℳ тогда и только тогда, когда для всех n ∈ ℕ:
$|Z_n| ≤ R$

На практике вычисляется до конечного числа итераций N.

В работе я принял N = 256 и R = 10

## Рассмотренные в работе варианты оптимизаций
1. Отсутствие оптимизации
2. Использование флагов компилятора

Я использую компилятор g++ с флагом:
```bash
-O3
```
3. Автоматическая векторизация компилятором

Вместо обработки по одной точке, начинаем обрабатывать сразу по 8 точек плоскости, в результате чего компилятор оптимизирует код.

4. Ручная векторизация при помощи intrinsic-ов

Также обрабатываем по 8 точек за раз, но теперь оптимизируем код вручную, используя intrinsic-и

## Сборка на Linux x86-64
Для сборки необходимо написать в коммандной строке
```bash
make
```

## Запуск
Для запуска программы необходимо написать в командной строке имя исполняемого файла
```bash
./do
```

## Взаимодействие с программой
После запуска программы будет открыто окно с изображением множества Мандельброта. Предусмотрена возможность взаимодействия:

- Перемещение по графику
    - ```W``` - вверх
    - ```A``` - влево
    - ```S``` - вниз
    - ```D``` - вправо

- Zoom графика
    - ```Z``` - увеличение
    - ```X``` - уменьшение
 
- Режимы работы
    - ```1``` - Отсутствие оптимизации
    - ```2``` - Оптимизация компилятором
    - ```3``` - Оптимизация intrinsic-ами

- Завершение программы
    - ```Q```
 
## Результаты оптимизаций
Для оценки эффективности оптимизаций будем использовать значение fps. 

Результаты получены при ограничении частоты процессора до 2Ghz, для окна размерами 800 на 800 пикселей, в начальном положении по центру экрана, со значением переменной scale = 4.

<table style="width: 100%; border-collapse: collapse;">
  
  <tr>
    <th style="background-color: #e6e6e6; border: 1px solid #ddd; padding: 8px; text-align: left;">
      -
    </th>
    <th style="background-color: #e6e6e6; border: 1px solid #ddd; padding: 8px; text-align: center;">
      Без использования флага -O3
    </th>
    <th style="background-color: #e6e6e6; border: 1px solid #ddd; padding: 8px; text-align: center;">
      С использованием флага -O3
    </th>
  </tr>
  
  <tr>
    <td style="background-color: #f9f9f9; border: 1px solid #ddd; padding: 8px; font-weight: bold;">
      Без оптимизаций
    </td>
    <td style="border: 1px solid #ddd; padding: 8px; text-align: center;">
      5
    </td>
    <td style="border: 1px solid #ddd; padding: 8px; text-align: center;">
      11
    </td>
  </tr>
  
  <tr>
    <td style="background-color: #f9f9f9; border: 1px solid #ddd; padding: 8px; font-weight: bold;">
      Векторизация компилятором
    </td>
    <td style="border: 1px solid #ddd; padding: 8px; text-align: center;">
      5
    </td>
    <td style="border: 1px solid #ddd; padding: 8px; text-align: center;">
      26
    </td>
  </tr>

  <tr>
    <td style="background-color: #f9f9f9; border: 1px solid #ddd; padding: 8px; font-weight: bold;">
      Векторизация intrinsic-ами
    </td>
    <td style="border: 1px solid #ddd; padding: 8px; text-align: center;">
      12
    </td>
    <td style="border: 1px solid #ddd; padding: 8px; text-align: center;">
      48
    </td>
  </tr>
  
</table>

