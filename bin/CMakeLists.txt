add_executable(labwork5 main.cpp)
target_link_libraries(labwork5 field game ship strategy)
target_include_directories(labwork5 PRIVATE ${PROJECT_SOURCE_DIR})