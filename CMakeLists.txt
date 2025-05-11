cmake_minimum_required(VERSION 3.28)
project("Tetris" LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(
    "Tetris"
    "src/main.cpp"
    "src/Utility.cpp"
    "src/Game.cpp"
    "src/Grid.cpp"
    "src/Tetromino.cpp"
    "src/TetrominoGenerator.cpp"
    "src/HUD.cpp"
    "src/TitleScreenShapes.cpp"
    "src/SoundManager.cpp")
target_compile_features("Tetris" PRIVATE cxx_std_17)

# Don't link SFML::Main on non-Windows platforms
if(WIN32)
    target_link_libraries("Tetris" PRIVATE SFML::Main SFML::System SFML::Window SFML::Graphics SFML::Audio)
else()
    target_link_libraries("Tetris" PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Audio)
endif()

target_compile_definitions("Tetris" PRIVATE SFML_STATIC)

# Prevent console window on Windows (does not affect Debug mode)
if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties("Tetris" PROPERTIES
        WIN32_EXECUTABLE TRUE)
endif()