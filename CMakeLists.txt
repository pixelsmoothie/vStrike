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
        global/states/modeSelection.h
        global/states/masterIncluder.h
        core/gameView.h
        core/localView.h
        core/aiView.h
        UI/fader.h
        global/customFont.h
        core/qBrain.h
        core/settingsOverlay.h
        UI/GameTimer.cpp
        UI/GameTimer.h
        global/ShapeHelpers.h
)
target_link_libraries(PongArena PRIVATE raylib)

# Automatically copy all assets to the build/executable directory on every single build
add_custom_command(TARGET PongArena POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:PongArena>/assets
)

if(EMSCRIPTEN)
    set_target_properties(PongArena PROPERTIES OUTPUT_NAME "index")
    set_target_properties(PongArena PROPERTIES SUFFIX ".html")
    target_link_options(PongArena PRIVATE
        "-sUSE_GLFW=3"
        "-sALLOW_MEMORY_GROWTH=1"
        "--preload-file" "${CMAKE_CURRENT_SOURCE_DIR}/assets@assets"
        "--shell-file" "${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
    )
endif()
