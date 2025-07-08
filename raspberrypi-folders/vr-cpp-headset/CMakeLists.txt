cmake_minimum_required(VERSION 3.10)
project(vr-cube)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

add_executable(vr-cube main.cpp uart.cpp)

target_include_directories(vr-cube PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(vr-cube
    ${SDL2_LIBRARIES}
    GLESv2
    EGL
)

# cmake_minimum_required(VERSION 3.10)
# project(vr_cube)

# # Set C++ standard
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Add source files
# set(SOURCES
#     main.cpp
#     uart.cpp
# )

# # Add executable
# add_executable(vr-cube ${SOURCES})

# # Link necessary libraries for Raspberry Pi and OpenGL ES 2.0
# # bcm_host is required for accessing the Pi's display
# # GLESv2 is OpenGL ES 2.0, EGL is for context management
# find_library(BCM_HOST_LIB bcm_host)
# find_library(GLESv2_LIB GLESv2)
# find_library(EGL_LIB EGL)

# # Link them to the executable
# target_link_libraries(vr-cube
#     ${BCM_HOST_LIB}
#     ${GLESv2_LIB}
#     ${EGL_LIB}
# )

# # Optionally include directories if needed
# # (Useful if you're including headers from /opt/vc or elsewhere)
# include_directories(/opt/vc/include)
# link_directories(/opt/vc/lib)