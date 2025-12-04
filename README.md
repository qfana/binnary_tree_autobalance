# Binary Tree Autobalance

Реализация красно-черного дерева с STL-совместимым интерфейсом.

## Сборка проекта

Проект использует CMake для сборки.

### Сборка на Linux/macOS:

```bash
# Создаем директорию для сборки
mkdir build
cd build

# Генерируем файлы сборки
cmake ..

# Компилируем проект
cmake --build .

# Или просто
make
```

### Сборка на Windows:

```powershell
# Создаем директорию для сборки
mkdir build
cd build

# Генерируем файлы сборки (для Visual Studio)
cmake ..

# Компилируем проект
cmake --build .

# Или откройте сгенерированный .sln файл в Visual Studio
```

### Запуск:

```bash
# Linux/macOS
./BinaryTreeAutobalance

# Windows
BinaryTreeAutobalance.exe
```

## Структура проекта

- `tree/tree.hpp` - Реализация красно-черного дерева и итератора
- `constructor_utils/constructor_utils.hpp` - Утилиты для конструкторов из файлов
- `main.cpp` - Примеры использования и тесты

