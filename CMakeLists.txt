cmake_minimum_required(VERSION 3.11)

project(TerminalAdventure CXX)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "TerminalAdventure")
set(DIR "sources")
set(SOURCES "${DIR}/main.cpp" "${DIR}/GameLogic/GameObjects.cpp" "${DIR}/GameLogic/GameObjects.h" "${DIR}/WindowAndGraphics/WindowTools.cpp" "${DIR}/WindowAndGraphics/WindowTools.h")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} -lncurses)

