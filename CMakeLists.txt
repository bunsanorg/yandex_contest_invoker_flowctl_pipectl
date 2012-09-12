project(yandex_contest_invoker_flowctl_pipectl)

include(${CMAKE_SOURCE_DIR}/system-config.cmake OPTIONAL)

cmake_minimum_required(VERSION 2.8)

include(YandexContestCommon)

# Targets
add_executable(${PROJECT_NAME}
    src/pipectl.c
)

# installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
