project(collector)
add_executable(${PROJECT_NAME} main.cpp window.cpp starlayers.cpp collector.cpp objects.cpp)
enable_abcg(${PROJECT_NAME})
