cmake_minimum_required(VERSION 2.8)
set(APP_NAME "capture-picture")

find_package(PkgConfig)
pkg_check_modules(OPENCV REQUIRED opencv)

cmake_policy(SET CMP0015 NEW)
AUX_SOURCE_DIRECTORY(src SRC_LIST)
add_executable(${APP_NAME} ${SRC_LIST})

include_directories(
    ${OPENCV_INCLUDE_DIRS}
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
target_link_libraries(${APP_NAME}
    ${OPENCV_LIBRARIES}
)

install(
    PROGRAMS ${PROJECT_BINARY_DIR}/${APP_NAME}
    DESTINATION bin
)