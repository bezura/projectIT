# projectIT
# **Отчёт о проекте: Взаимодействие с окружающей средой посредством датчиков и выводом соответствующих образов**

### Выполнил: _Хрусталев Влад Николаевич_
>Группа: _НПИбд-02-22_
Студенческий билет № 1132222006

### Руководитель: _Виноградов Андрей Николаевич_
> к.ф.-м.н., доцент кафедры информационных технологий

#### 2023 год

# **1. Введение:**
Проект "Интерактивное взаимодействие с окружающей средой на основе датчиков и светодиодной матрицы" представляет собой устройство, способное взаимодействовать с окружающей средой с помощью ультразвукового датчика расстояния и датчика освещенности. Оно выводит соответствующие образы и реагирует на изменения, создавая уникальные эффекты и анимацию. При помощи платы Arduino и светодиодной матрицы, проект становится более гибким и многофункциональным, предлагая различные возможности взаимодействия с окружающим миром.

# **2. Подключение и сборка:**
Для реализации проекта необходимо подключить следующие компоненты:
- Arduino или совместимая плата (в данном проекте используется плата NodeMCU на esp32-wroom32d)
- Ультразвуковой датчик расстояния (подключение пина TRIG к пину 14, а пина ECHO к пину 13 на Arduino)
- Датчик освещенности (подключение пина LIGHT_SENSOR к аналоговому пину 0 на Arduino)
- Светодиодную матрицу(16x16) с 256 светодиодами (DATA_PIN подключен к пину 5)

Ниже приведена схема подключения:

[![Scheme|50%](https://i.ibb.co/0chfFnV/image.png) ](https://ibb.co/fqpwnRW)

Необходимо также установить необходимые библиотеки для работы с устройствами и эффектами анимации, такие как GyverTimer и FastLED.

>Пару кадров из собранного устройства:
>Собранная схема на макетной плате.
>![1](https://github.com/bezura/projectIT/blob/main/frames_to_pdf/1.jpg?raw=true)
>Вывод матрицы статичного изображения без рассеивателя
>![2](https://github.com/bezura/projectIT/blob/main/frames_to_pdf/2.jpg?raw=true)
>Вывод матрицы статичного изображения с рассеивателем в виде листочка
>![3](https://github.com/bezura/projectIT/blob/main/frames_to_pdf/3.jpg?raw=true)

Исходя из изображений с рассеивателем и без делаем вывод, что желательно исползовать какой-либо плотный материал пропускающий свет для улучшения восприятия.
# **3. Работа программы:**
>*Программа на Arduino использует таймеры для асинхронной работы и предотвращения блокировки выполнения кода. Таймеры позволяют устройству проверять состояние датчиков и обновлять отображение на светодиодной матрице без простоя.*

После подключения и загрузки программы на Arduino, устройство начинает взаимодействовать с окружающей средой на основе данных, полученных от датчиков. Работа программы состоит из следующих состояний и условий вызова функций:

- **Режим сна (Sleep):** Если датчик освещенности обнаруживает низкий уровень освещенности (срабатывает функция `Light_on()`), то устройство переходит в режим сна. В этом режиме вызывается функция `Sleep()`, которая отображает анимацию сна на светодиодной матрице. Анимация включает несколько кадров, которые последовательно отображаются на матрице. После выполнения анимации устройство возвращается в исходное состояние.

- **Режим подмигивания (Smile):** Если ничего со средой не происходит (датчики не улавливают нужных изменений) и срабатывает таймер `Time_Smile()`, устройство переходит в режим подмигивания. В этом режиме вызывается функция `Smile()`, которая отображает анимацию подмигивания на светодиодной матрице. Анимация включает несколько кадров, которые последовательно отображаются на матрице. После выполнения анимации устройство возвращается в исходное состояние.

- **Режим удивления (Wow):** Если ультразвуковой датчик расстояния обнаруживает близкое препятствие (срабатывает функция `CheckDist()`), устройство переходит в режим удивления. В этом режиме вызывается функция `Wow()`, которая отображает анимацию удивления на светодиодной матрице. Анимация включает несколько кадров, которые последовательно отображаются на матрице. После выполнения анимации устройство возвращается в исходное состояние на время заданное в таймере `Timer_Off_Wow()`.

Программа работает в бесконечном цикле, постоянно проверяя состояние окружающей среды с помощью датчиков и выполняя соответствующие действия и анимации на светодиодной матрице. Обновление отображения на матрице происходит с помощью функции `FastLED.show()` при срабатывании таймера `Timer_Frames`.

Из-за больших размеров кода программы, он будет в **Приложении 1**.

```cpp
//ПРИЛОЖЕНИЕ 1
```

>В коде программы используется файл `effects.h`, в котором хранятся битмапы для различных эффектов, включая смайлы. Битмапы хранятся во flash-памяти с помощью макроса `PROGMEM`, что позволяет эффективно использовать ресурсы памяти микроконтроллера.

Из-за больших размеров содержимое файла `effects.h` будет в **Приложении 2**.

```cpp
//ПРИЛОЖЕНИЕ 2
```

Для создания картинок, которые в дальнейшем  будут составлять анимацию, был использован сайт [PixelArt](pixilart.com/draw), котррый идеяльно подошел для рисования рисунков 16x16(по клеткам).

Далее для упрощения и оптимизации преобразования картинок в массивы было решено написать программу на `Python`. Содержимое файла `converter.py` будет в **Приложении 3**. 
```py
#ПРИЛОЖЕНИЕ 3
```
>Программа имеет детальные поясления прямо в коде, но в кратце она создает двухмерный массив с названием папки, после этого в этот массив вписываются массивы созданные из картинок(должны быть в формате *bmp*) в этой папке , состоящие 256 элементов, каждый из которых является 24-битной кодировки цвета соответсвующего пикселя.

Так же посмотреть/скачать проект со всеми файлами разрабоки(картинки, программы) можно в моём GitHub 
[![GitHub Badge](https://img.shields.io/badge/Go_to-GitHub-red)](https://github.com/bezura/projectIT)


# **4. Выводы:**
Проект "Взаимодействие с окружающей средой посредством датчиков и выводом соответствующих образов" успешно реализован. Данное устройство может стать полноценной частью интерьера комнаты, игрушкой или ночником, добавляя интерактивность и оживление в окружающее пространство. Дальнейшая разработка данного устройства и его интеграция в корпус позволит создать еще более привлекательное и функциональное устройство, способное взаимодействовать с окружающей средой и служить в качестве индикатора или декоративного элемента.
