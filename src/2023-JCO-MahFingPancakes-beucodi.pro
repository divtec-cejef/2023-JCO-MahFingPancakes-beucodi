#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui

QT += svgwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2023-JCO-MahFingPancakes-beucodi
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BRECT
#DEFINES += DEBUG_SHAPE
#DEFINES += DEPLOY # Pour une compilation dans un but de déploiement

SOURCES += main.cpp\
    mainfrm.cpp \
    gamescene.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    gameview.cpp \
    utilities.cpp \
    gamecanvas.cpp \
    spritetickhandler.cpp \
    body.cpp \
    door.cpp \
    enemy.cpp \
    entity.cpp \
    fallingplatform.cpp \
    finalboss.cpp \
    fragileplatform.cpp \
    heartcharge.cpp \
    hearticon.cpp \
    hudicon.cpp \
    item.cpp \
    jumpcharge.cpp \
    jumpicon.cpp \
    jumpingpancake.cpp \
    level.cpp \
    levelbuilder.cpp \
    mindsignal.cpp \
    platform.cpp \
    regenpod.cpp \
    solidplatform.cpp \
    transparentplatform.cpp \
    player.cpp \
    flyingpancake.cpp

HEADERS  += mainfrm.h \
    gamescene.h \
    sprite.h \
    gamecore.h \
    resources.h \
    gameview.h \
    utilities.h \
    gamecanvas.h \
    spritetickhandler.h \
    body.h \
    door.h \
    enemy.h \
    entity.h \
    fallingplatform.h \
    finalboss.h \
    fragileplatform.h \
    heartcharge.h \
    hearticon.h \
    hudicon.h \
    item.h \
    jumpcharge.h \
    jumpicon.h \
    jumpingpancake.h \
    level.h \
    levelbuilder.h \
    mindsignal.h \
    platform.h \
    regenpod.h \
    solidplatform.h \
    transparentplatform.h \
    player.h \
    flyingpancake.h
    
FORMS    += mainfrm.ui
