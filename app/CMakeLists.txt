add_subdirectory(weibo_test)
add_subdirectory(dominant_color)
add_subdirectory(capture_picture)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/resources/common
    DESTINATION ${RESOURCE_DIR}
    )
