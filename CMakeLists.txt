cmake_minimum_required(VERSION 3.0)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

project(yandex_contest_invoker_flowctl_pipectl)

if(BunsanCMakeBuiltin)
    BunsanCMakeLoad()
else()
    find_package(BunsanCMake CONFIG REQUIRED)
endif()

bunsan_add_executable(${PROJECT_NAME}
    src/pipectl.c
)

bunsan_install_targets(${PROJECT_NAME})
bunsan_install_project()
