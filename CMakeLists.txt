cmake_minimum_required(VERSION 3.21)

project(16_SFML_Games LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SFML_GAMES_BUILD_TESTS "Build the unit tests" ON)

include(FetchContent)

# ---------------------------------------------------------------------------
# SFML 3: use an installed copy if one is found, otherwise download it.
#   Point CMake at an installed SFML with -DSFML_DIR=<sfml>/lib/cmake/SFML
#   or -DCMAKE_PREFIX_PATH=<sfml>. Force the download with
#   -DSFML_GAMES_FETCH_SFML=ON.
# ---------------------------------------------------------------------------
option(SFML_GAMES_FETCH_SFML "Always download and build SFML instead of searching for it" OFF)

if(NOT SFML_GAMES_FETCH_SFML)
    find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
endif()

if(SFML_FOUND)
    message(STATUS "Using installed SFML ${SFML_VERSION}")
else()
    message(STATUS "SFML not found - fetching SFML 3.0.2")
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.2
        GIT_SHALLOW    ON
    )
    FetchContent_MakeAvailable(SFML)
endif()

# ---------------------------------------------------------------------------
# Game launcher
# ---------------------------------------------------------------------------
set(GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/16_SFML_Games)

add_executable(16_SFML_Games
    ${GAME_DIR}/16_SFML_Games.cpp
    ${GAME_DIR}/arkanoid.cpp
    ${GAME_DIR}/asteroids.cpp
    ${GAME_DIR}/bejeweled.cpp
    ${GAME_DIR}/chess.cpp
    ${GAME_DIR}/doodle_jump.cpp
    ${GAME_DIR}/fifteen_puzzle.cpp
    ${GAME_DIR}/mahjong.cpp
    ${GAME_DIR}/minesweeper.cpp
    ${GAME_DIR}/netwalk.cpp
    ${GAME_DIR}/outrun.cpp
    ${GAME_DIR}/racing.cpp
    ${GAME_DIR}/snake.cpp
    ${GAME_DIR}/tetris.cpp
    ${GAME_DIR}/tron.cpp
    ${GAME_DIR}/xonix.cpp
    # volleyball.cpp needs Box2D and is not built yet (see TODO in 16_SFML_Games.cpp)
    ${GAME_DIR}/Connector.hpp
    ${GAME_DIR}/Grid.h
    ${GAME_DIR}/Player.h
    ${GAME_DIR}/XonixConstants.h
)

target_link_libraries(16_SFML_Games PRIVATE SFML::Graphics SFML::Window SFML::System)

# Put the executable in <build>/bin (plus a config subfolder for multi-config generators)
set_target_properties(16_SFML_Games PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Games load assets via relative paths ("images/..."), so copy them next to the exe.
add_custom_command(TARGET 16_SFML_Games POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${GAME_DIR}/images $<TARGET_FILE_DIR:16_SFML_Games>/images
    COMMENT "Copying game images"
)

# Bundled chess engine is a Windows binary; on other platforms install stockfish via your package manager.
if(WIN32 AND EXISTS ${GAME_DIR}/stockfish.exe)
    add_custom_command(TARGET 16_SFML_Games POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${GAME_DIR}/stockfish.exe $<TARGET_FILE_DIR:16_SFML_Games>
    )
endif()

# Linux / macOS: look for an installed stockfish (not downloaded). Debian/Ubuntu put it in
# /usr/games, which is often not on PATH, so the full path found here is baked into the game.
if(NOT WIN32)
    find_program(STOCKFISH_EXECUTABLE stockfish
        HINTS /usr/games /usr/local/games /opt/homebrew/bin /usr/local/bin
    )
    if(STOCKFISH_EXECUTABLE)
        message(STATUS "Found Stockfish: ${STOCKFISH_EXECUTABLE}")
        target_compile_definitions(16_SFML_Games PRIVATE
            SFML_GAMES_STOCKFISH_PATH="${STOCKFISH_EXECUTABLE}"
        )
    else()
        message(WARNING
            "Stockfish not found - chess will run but the computer cannot move.\n"
            "Install it, then re-run CMake:\n"
            "  Debian/Ubuntu: sudo apt install stockfish\n"
            "  Fedora:        sudo dnf install stockfish\n"
            "  Arch:          sudo pacman -S stockfish\n"
            "  macOS:         brew install stockfish\n"
            "Or point CMake at it with -DSTOCKFISH_EXECUTABLE=/path/to/stockfish"
        )
    endif()
endif()

# An installed, shared SFML on Windows needs its DLLs beside the exe.
if(WIN32 AND SFML_FOUND AND NOT SFML_STATIC_LIBRARIES)
    add_custom_command(TARGET 16_SFML_Games POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:16_SFML_Games> $<TARGET_FILE_DIR:16_SFML_Games>
        COMMAND_EXPAND_LISTS
    )
endif()

# ---------------------------------------------------------------------------
# Catcher: the worked example in howto_split.md, before and after the split
# ---------------------------------------------------------------------------
set(CATCHER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/howto_split)

add_executable(catcher_before ${CATCHER_DIR}/before/catcher.cpp)
add_executable(catcher_after  ${CATCHER_DIR}/after/catcher.cpp ${CATCHER_DIR}/after/CatcherGame.h)

foreach(catcher catcher_before catcher_after)
    target_link_libraries(${catcher} PRIVATE SFML::Graphics SFML::Window SFML::System)
    set_target_properties(${catcher} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    if(WIN32 AND SFML_FOUND AND NOT SFML_STATIC_LIBRARIES)
        add_custom_command(TARGET ${catcher} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_RUNTIME_DLLS:${catcher}> $<TARGET_FILE_DIR:${catcher}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endforeach()

# ---------------------------------------------------------------------------
# Unit tests (GoogleTest)
# ---------------------------------------------------------------------------
if(SFML_GAMES_BUILD_TESTS)
    enable_testing()

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)   # match MSVC's default /MD runtime
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.15.2
        GIT_SHALLOW    ON
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(Tests Tests/test.cpp)
    target_include_directories(Tests PRIVATE ${GAME_DIR})
    target_link_libraries(Tests PRIVATE GTest::gtest_main)

    add_executable(CatcherTests ${CATCHER_DIR}/after/CatcherGame_test.cpp)
    target_include_directories(CatcherTests PRIVATE ${CATCHER_DIR}/after)
    target_link_libraries(CatcherTests PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(Tests)
    gtest_discover_tests(CatcherTests TEST_PREFIX "Catcher.")
endif()
