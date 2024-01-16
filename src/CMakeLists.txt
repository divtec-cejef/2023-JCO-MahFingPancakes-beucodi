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
        solidplatform.cpp
        fallingplatform.cpp
        body.cpp
        fragileplatform.cpp
        hudicon.cpp
        jumpcharge.cpp
        level.cpp
        door.cpp
        levelbuilder.cpp
        enemy.cpp
        entity.cpp
        hearticon.cpp
        jumpingpancake.cpp
        jumpingpancake.h
        door.h
        body.h
        enemy.h
        entity.h
        fallingplatform.h
        fragileplatform.h
        gamecanvas.h
        gamecore.h
        gamescene.h
        gameview.h
        hearticon.h
        hudicon.h
        jumpcharge.h
        level.h
        levelbuilder.h
        mainfrm.h
        platform.h
        player.h
        resources.h
        solidplatform.h
        sprite.h
        spritetickhandler.h
        transparentplatform.h
        utilities.h
        mindsignal.cpp
        mindsignal.h)

target_link_libraries(MahFingPancakes PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        Qt::SvgWidgets
)
