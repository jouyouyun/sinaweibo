SET(NAME "weibo-test")

GEN_JSC_SOURCE(JSC_GEN "jsc")

SET(SRC_LIST main.c weibo.c weibo_dbus.c ${JSC_GEN})

add_executable(${NAME} ${SRC_LIST})

find_package(PkgConfig)
pkg_check_modules(DBUS_GLIB REQUIRED dbus-glib-1)
target_link_libraries(${NAME} dcom dentry ${DWEBVIEW_LIBRARIES})

include_directories(
	${DWEBVIEW_INCLUDE_DIRS}
	${DBUS_GLIB_INCLUDE_DIRS}
)
include_directories(
	${DTEST_INCLUDE_DIRS}
	${DBUS_GLIB_INCLUDE_DIRS}
)

execute_process(
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/resources/${NAME}/js
    )

install(
    PROGRAMS ${PROJECT_BINARY_DIR}/${NAME}
    DESTINATION bin
    )
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/resources/${NAME}
    DESTINATION ${RESOURCE_DIR}
    )

GEN_JS_FILE(${NAME})