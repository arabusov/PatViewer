cmake_minimum_required(VERSION 3.1.0)
project (gui)
# change path to local qt install
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package (Boost COMPONENTS program_options REQUIRED)
file (GLOB display_src "src/*.cpp")
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Declarative)
include_directories ( ${Boost_INCLUDE_DIR} include ../Pattern/include
${Qt5Widgets_INCLUDE_DIRS})
add_library (gui SHARED ${display_src})

