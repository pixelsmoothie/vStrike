cmake_minimum_required(VERSION 3.20)
project(PongArena CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Override policy minimum to fix compatibility errors in external dependencies (like Raylib)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

# Keep raylib build minimal
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

add_executable(PongArena main.cpp
        entities/paddle.h
        entities/paddle.cpp
        global/constants.h
        entities/ball.h
        entities/ball.cpp
        physics/physicsEngine.h
        physics/physicsEngine.cpp
        UI/HealthBar.h
        UI/HealthBar.cpp
        global/states/gameStates.h
        global/states/gameScreen.h
        global/states/menuScreen.h
        global/states/settingScreen.h
)
target_link_libraries(PongArena PRIVATE raylib)
