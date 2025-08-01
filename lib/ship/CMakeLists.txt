add_library(ship ship.h ship.cpp)

target_include_directories(ship PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(ship PRIVATE field)