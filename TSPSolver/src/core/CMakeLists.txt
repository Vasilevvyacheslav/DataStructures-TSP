add_library(core_module STATIC
    TSPGraph.h
    TSPGraph.cpp
    BranchAndBound.h
    branchandbound.cpp
)
target_include_directories(core_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
