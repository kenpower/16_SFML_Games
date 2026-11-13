cmake_minimum_required(VERSION 3.21)

project(16_SFML_Games LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SFML_GAMES_BUILD_TESTS "Build the unit tests" ON)
# OFF configures only the unit tests: no SFML is needed, so it is quick (used by CI),
# and any class under test that includes SFML fails to compile.
option(SFML_GAMES_BUILD_GAME "Build the game launcher (requires SFML)" ON)

include(FetchContent)

set(GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/16_SFML_Games)
set(CATCHER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/howto_split)

if(SFML_GAMES_BUILD_GAME)
# ---------------------------------------------------------------------------
# SFML 3: use an installed copy if a usable one is found, otherwise download it.
#
# Searched, in order:
#   -DSFML_DIR=<sfml>/lib/cmake/SFML, -DSFML_ROOT=<sfml> or env SFML_ROOT,
#   -DCMAKE_PREFIX_PATH=<sfml> or env CMAKE_PREFIX_PATH,
#   -DSFML_SDK=<sfml> or env SFML_SDK (an SDK folder, or a folder holding SFML-3* SDKs),
#   C:\SFML-3*, C:\SFML-3*\SFML-3*, C:\SFML\SFML-3* and C:\SFML (Windows, newest first),
#   then CMake's usual places (PATH, Program Files, /usr, /usr/local, Homebrew).
#
# A candidate is only used if a small test program builds and links against it,
# so an SDK for the wrong compiler or architecture is skipped rather than
# breaking the build. Force the download with -DSFML_GAMES_FETCH_SFML=ON.
# ---------------------------------------------------------------------------
option(SFML_GAMES_FETCH_SFML "Always download and build SFML instead of searching for it" OFF)
set(SFML_SDK "" CACHE PATH "SFML 3 SDK folder, or a folder containing SFML-3* SDKs (env SFML_SDK also works)")

set(SFML_GAMES_USE_INSTALLED OFF)

if(NOT SFML_GAMES_FETCH_SFML)
    # Extra places to look, beyond what find_package searches by itself.
    set(_sfml_roots)
    foreach(_root IN ITEMS "${SFML_SDK}" "$ENV{SFML_SDK}")
        if(_root)
            file(TO_CMAKE_PATH "${_root}" _root)
            list(APPEND _sfml_roots "${_root}")
            file(GLOB _sub LIST_DIRECTORIES true "${_root}/SFML-3*")
            list(SORT _sub COMPARE NATURAL ORDER DESCENDING)
            list(APPEND _sfml_roots ${_sub})
        endif()
    endforeach()
    if(WIN32)
        # C:\SFML-3.x, its "Extract All" nested form C:\SFML-3.x\SFML-3.x,
        # C:\SFML\SFML-3.x, and an SDK renamed to plain C:\SFML.
        file(GLOB _sub LIST_DIRECTORIES true
            "C:/SFML-3*" "C:/SFML-3*/SFML-3*" "C:/SFML/SFML-3*")
        list(SORT _sub COMPARE NATURAL ORDER DESCENDING)
        list(APPEND _sfml_roots ${_sub} "C:/SFML")
    endif()
    set(_sfml_hints)
    foreach(_root IN LISTS _sfml_roots)
        if(IS_DIRECTORY "${_root}")
            list(APPEND _sfml_hints "${_root}")
        endif()
    endforeach()
    list(REMOVE_DUPLICATES _sfml_hints)

    # Re-check only when something that affects the result has changed.
    set(_sfml_sig "${_sfml_hints}|${SFML_DIR}|${SFML_ROOT}|$ENV{SFML_ROOT}|${CMAKE_PREFIX_PATH}|$ENV{CMAKE_PREFIX_PATH}|${CMAKE_CXX_COMPILER}|${CMAKE_GENERATOR_PLATFORM}|${SFML_STATIC_LIBRARIES}")

    if(NOT _sfml_sig STREQUAL SFML_GAMES_SFML_CHECKED)
        message(STATUS "Looking for a usable SFML 3")
        string(REPLACE ";" "|" _hints_arg "${_sfml_hints}")
        string(REPLACE ";" "|" _prefix_arg "${CMAKE_PREFIX_PATH}")
        set(_check_flags
            "-DSFML_HINTS=${_hints_arg}"
            "-DSFML_PREFIX=${_prefix_arg}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        )
        foreach(_var IN ITEMS SFML_DIR SFML_ROOT SFML_STATIC_LIBRARIES CMAKE_TOOLCHAIN_FILE)
            if(${_var})
                list(APPEND _check_flags "-D${_var}=${${_var}}")
            endif()
        endforeach()

        set(_sfml_dir_file ${CMAKE_BINARY_DIR}/sfml_check/sfml_dir.txt)
        file(REMOVE ${_sfml_dir_file})
        try_compile(_sfml_ok
            ${CMAKE_BINARY_DIR}/sfml_check
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sfml_check
            sfml_check
            CMAKE_FLAGS ${_check_flags}
        )

        set(_sfml_found_dir "")
        if(EXISTS ${_sfml_dir_file})
            file(READ ${_sfml_dir_file} _sfml_found_dir)
        endif()

        if(_sfml_ok AND _sfml_found_dir)
            set(SFML_DIR "${_sfml_found_dir}" CACHE PATH "Folder containing SFMLConfig.cmake" FORCE)
            set(SFML_GAMES_SFML_USABLE ON CACHE INTERNAL "")
        else()
            set(SFML_GAMES_SFML_USABLE OFF CACHE INTERNAL "")
            if(_sfml_found_dir)
                message(STATUS "SFML found at ${_sfml_found_dir} but it does not build with this "
                               "compiler/architecture (wrong SDK download?) - ignoring it")
            endif()
        endif()

        # Recompute: SFML_DIR may have just been set.
        set(_sfml_sig "${_sfml_hints}|${SFML_DIR}|${SFML_ROOT}|$ENV{SFML_ROOT}|${CMAKE_PREFIX_PATH}|$ENV{CMAKE_PREFIX_PATH}|${CMAKE_CXX_COMPILER}|${CMAKE_GENERATOR_PLATFORM}|${SFML_STATIC_LIBRARIES}")
        set(SFML_GAMES_SFML_CHECKED "${_sfml_sig}" CACHE INTERNAL "")
    endif()

    if(SFML_GAMES_SFML_USABLE)
        find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
        set(SFML_GAMES_USE_INSTALLED ${SFML_FOUND})
    endif()
endif()

if(SFML_GAMES_USE_INSTALLED)
    message(STATUS "Using installed SFML ${SFML_VERSION} from ${SFML_DIR}")
else()
    set(SFML_FOUND FALSE)
    message(STATUS "No usable installed SFML 3 - fetching SFML 3.0.2")
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
endif() # SFML_GAMES_BUILD_GAME

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

    # Every .cpp in Tests/ is part of the test program, so a new test file
    # (e.g. Tests/tetris_board_test.cpp) only needs CMake to be re-run.
    # The Tests target does NOT link SFML: classes under test must not include it.
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Tests/*.cpp)
    add_executable(Tests ${TEST_SOURCES})
    target_include_directories(Tests PRIVATE ${GAME_DIR})
    target_link_libraries(Tests PRIVATE GTest::gtest_main)

    add_executable(CatcherTests ${CATCHER_DIR}/after/CatcherGame_test.cpp)
    target_include_directories(CatcherTests PRIVATE ${CATCHER_DIR}/after)
    target_link_libraries(CatcherTests PRIVATE GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(Tests)
    gtest_discover_tests(CatcherTests TEST_PREFIX "Catcher.")
endif()
