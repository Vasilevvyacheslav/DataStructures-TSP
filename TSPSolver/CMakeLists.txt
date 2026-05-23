cmake_minimum_required(VERSION 3.19)
project(TSPSolver LANGUAGES CXX)

# Современная настройка Qt6 (из твоего первого файла)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

# Модульная структура (из второго файла)
add_subdirectory(src)
