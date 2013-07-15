project(yandex_contest_invoker_flowctl_pipectl)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(BunsanCommon)

bunsan_add_executable(${PROJECT_NAME}
    src/pipectl.c
)

bunsan_install_targets(${PROJECT_NAME})
