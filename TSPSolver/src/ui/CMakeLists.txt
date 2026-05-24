add_library(ui_module STATIC
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    TSPVisualizer.h
    TSPVisualizer.cpp
)
target_link_libraries(ui_module PUBLIC core_module Qt6::Widgets)
target_include_directories(ui_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
