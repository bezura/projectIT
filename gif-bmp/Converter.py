import os
from PIL import Image

# Функция преобразования картинки в массив
def image_to_array(image):
    # Преобразование размера картинки в 16x16
    resized_image = image.resize((16, 16))
    
    # Инициализация массива
    array = []
    
    # Преобразование картинки в режим RGB
    rgb_image = resized_image.convert('RGB')
    
    # Итерация по пикселям картинки
    for y in range(16):
        row = []
        if y % 2 == 0:
            # Четная строка (справа налево)
            for x in range(15, -1, -1):
                # Получение RGB значения пикселя
                r, g, b = rgb_image.getpixel((x, y))
                
                # Преобразование RGB значения в 24-битный код цвета
                color_code = (r << 16) | (g << 8) | b
                
                # Добавление кода цвета в строку
                row.append(color_code)
        else:
            # Нечетная строка (слева направо)
            for x in range(16):
                # Получение RGB значения пикселя
                r, g, b = rgb_image.getpixel((x, y))
                
                # Преобразование RGB значения в 24-битный код цвета
                color_code = (r << 16) | (g << 8) | b
                
                # Добавление кода цвета в строку
                row.append(color_code)
        
        # Добавление строки в массив
        array.append(row)
    
    return array

# Получение текущей директории программы
current_dir = os.path.dirname(os.path.abspath(__file__))

# Получение списка папок в текущей директории
folders = [folder for folder in os.listdir(current_dir) if os.path.isdir(folder)]

# Итерация по папкам
for folder in folders:
    # Создание пути к текущей папке
    folder_path = os.path.join(current_dir, folder)
    
    # Получение списка картинок в текущей папке
    images = [image for image in os.listdir(folder_path) if image.endswith('.bmp')]
    
    # Инициализация массива кодов картинок
    image_array = []
    
    # Итерация по картинкам
    for image_file in images:
        # Создание пути к текущей картинке
        image_path = os.path.join(folder_path, image_file)
        
        # Открытие картинки
        image = Image.open(image_path)
        
        # Преобразование картинки в массив и добавление его в массив кодов картинок
        image_array.append(image_to_array(image))
    
    # Генерация кода массива картинок
    array_code = 'const uint32_t {}[][256] PROGMEM = {{\n'.format(folder)
    for image_code in image_array:
        array_code += '  {\n'
        for row in image_code:
            array_code += '     '
            array_code += ', '.join(['0x{:06X}'.format(color) for color in row])
            array_code += ' ,\n'
        array_code += '  },\n'
    array_code += '};\n'
    
    # Вывод кода массива картинок
    print(array_code)
