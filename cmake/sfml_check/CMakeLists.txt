# Probe project run by try_compile from the top-level CMakeLists.txt.
# It finds SFML 3 the same way the real build will and links a tiny program
# against it, so an SDK built for a different compiler is rejected up front.
cmake_minimum_required(VERSION 3.21)
project(sfml_check LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Lists arrive with '|' in place of ';' (try_compile splits on ';').
string(REPLACE "|" ";" SFML_HINTS "${SFML_HINTS}")
if(SFML_PREFIX)
    string(REPLACE "|" ";" SFML_PREFIX "${SFML_PREFIX}")
    list(APPEND CMAKE_PREFIX_PATH ${SFML_PREFIX})
endif()

# Not REQUIRED: a configure error here would abort the parent configure.
# When SFML is missing the probe fails at build time instead.
find_package(SFML 3 COMPONENTS Graphics Window System QUIET HINTS ${SFML_HINTS})

if(SFML_FOUND)
    # Tell the parent project which SFML was used.
    file(WRITE "${CMAKE_BINARY_DIR}/sfml_dir.txt" "${SFML_DIR}")
    add_executable(sfml_check main.cpp)
    target_link_libraries(sfml_check PRIVATE SFML::Graphics SFML::Window SFML::System)
else()
    add_custom_target(sfml_check ALL COMMAND ${CMAKE_COMMAND} -E false)
endif()
