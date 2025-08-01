add_library(field field.h field.cpp)

target_include_directories(field PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(field PRIVATE ship)