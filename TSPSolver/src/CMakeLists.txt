# Подключаем подпапки с библиотеками
add_subdirectory(core)
add_subdirectory(ui)

# Создаем исполняемый файл из main.cpp
add_executable(${PROJECT_NAME} main.cpp)

# Подключаем библиотеки к main.cpp
target_link_libraries(${PROJECT_NAME} PRIVATE
    core_module
    ui_module
    Qt${QT_VERSION_MAJOR}::Widgets
)
