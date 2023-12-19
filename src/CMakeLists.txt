cmake_minimum_required(VERSION 3.24)
project(MahFingPancakes)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC_PATH_PREFIX ON)
set(CMAKE_PREFIX_PATH "~/Qt/6.6.0/macos/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        SvgWidgets
        REQUIRED)

qt_standard_project_setup()


qt_add_executable(MahFingPancakes main.cpp
        mainfrm.cpp
        gamecanvas.cpp
        gamecore.cpp
        gamescene.cpp
        gameview.cpp
        resources.cpp
        sprite.cpp
        spritetickhandler.cpp
        utilities.cpp
        platform.cpp
        player.cpp
        transparentplatform.cpp
        transparentplatform.h
        solidplatform.cpp
        solidplatform.h
        fallingplatform.cpp
        fallingplatform.h
        body.cpp
        body.h
        fragileplatform.cpp
        fragileplatform.h
        hudicon.cpp
        hudicon.h
        jumpcharge.cpp
        jumpcharge.h
        level.cpp
        level.h)

target_link_libraries(MahFingPancakes PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        Qt::SvgWidgets
)
