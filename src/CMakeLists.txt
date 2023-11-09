cmake_minimum_required(VERSION 3.24)
project(MahFingPancakes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC_PATH_PREFIX ON)
set(CMAKE_PREFIX_PATH "~/Qt/6.6.0/macos/lib/cmake")

file(GLOB all_SRCS "*.cpp")
list(FILTER all_SRCS EXCLUDE REGEX "[a-zA-Z]*_blank\\.cpp$")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        SvgWidgets
        REQUIRED)


add_executable(MahFingPancakes ${all_SRCS})
target_link_libraries(MahFingPancakes
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        Qt::SvgWidgets
)