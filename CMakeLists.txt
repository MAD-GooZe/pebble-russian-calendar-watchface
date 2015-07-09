cmake_minimum_required(VERSION 3.2)
project(cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

set(PEBBLE_SDK_DIR /opt/pebble-dev/)

include_directories(${PEBBLE_SDK_DIR}/Pebble/basalt/include)

set(
    PEBBLE_HEADERS
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/gcolor_definitions.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/pebble_fonts.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/pebble.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/pebble_process_info.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/pebble_warn_unsupported_functions.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/pebble_worker.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/pebble_fonts.h
    ${PEBBLE_SDK_DIR}/Pebble/basalt/include/doxygen/pebble_worker.h
)

set(
    SOURCE_FILES
    ${PEBBLE_HEADERS}
    src/RussianCalendar.c
    appinfo.json
    wscript
)

add_executable(tmp ${SOURCE_FILES})

add_custom_target(
    RussianCalender
    /opt/pebble-dev/bin/pebble build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)