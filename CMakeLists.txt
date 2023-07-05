cmake_minimum_required(VERSION 3.0.0)
project(Mode7 VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

# Find packages, the following line is only necessary for VCPKG users.
set(VCPKG_TARGET_TRIPLET x64-windows)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(minifb CONFIG REQUIRED)

add_executable(Mode7 main.c)
target_link_libraries(Mode7 PRIVATE minifb::minifb)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
