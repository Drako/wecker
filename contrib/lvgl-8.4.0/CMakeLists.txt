# this is not the original CMakeLists.txt from lvgl, but a custom one for this project

file(GLOB_RECURSE lvgl_HEADERS *.h)
file(GLOB_RECURSE lvgl_SOURCES *.c)

add_library(lvgl STATIC "${lvgl_SOURCES}" "${lvgl_HEADERS}")
target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE=1)
target_include_directories(lvgl PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
