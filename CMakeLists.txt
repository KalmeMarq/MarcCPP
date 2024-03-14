cmake_minimum_required(VERSION 3.18)
project(TestCMake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

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

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG        v1.14
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(pugixml)

target_link_libraries(TestCMake PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(TestCMake PRIVATE pugixml::pugixml)