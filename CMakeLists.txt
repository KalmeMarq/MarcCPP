cmake_minimum_required(VERSION 3.1...3.28)
project(TestCMake LANGUAGES CXX)

add_subdirectory(libraries/json)
add_subdirectory(libraries/pugixml)

add_executable(
    TestCMake
    src/main.cpp
    src/Marc.cpp
    src/Record.cpp
    src/Leader.cpp
    src/VariableField.cpp
    src/ControlField.cpp
    src/DataField.cpp
    src/Subfield.cpp
)

target_link_libraries(TestCMake PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(TestCMake PRIVATE pugixml::pugixml)