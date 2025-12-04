cmake_minimum_required(VERSION 3.10)
project(RedBlackTreeVariantD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/tree.cpp
    src/main.cpp
)

# Заголовочные файлы
set(HEADERS
    include/tree.h
    include/iterator.hpp
    include/constructor_utils.hpp
)

# Создаем исполняемый файл
add_executable(rbt_variant_d ${SOURCES} ${HEADERS})

# Устанавливаем пути к заголовочным файлам
target_include_directories(rbt_variant_d PRIVATE include)

# Настройки компиляции
if(MSVC)
    target_compile_options(rbt_variant_d PRIVATE /W4 /WX)
else()
    target_compile_options(rbt_variant_d PRIVATE -Wall -Wextra -Werror -pedantic)
endif()