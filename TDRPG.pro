#-------------------------------------------------
#
# Project created by QtCreator 2015-05-12T22:11:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TDRPG
TEMPLATE = app


SOURCES += main.cpp \
    Map.cpp \
    Game.cpp \
    Grid.cpp \
    Node.cpp \
    Sprite.cpp \
    Terrain.cpp \
    Edge.cpp \
    Graph.cpp \
    PathGrid.cpp \
    PathingMap.cpp \
    Tree.cpp \
    Tests/TestPathGrid.cpp \
    Tests/TestPathingMap.cpp \
    Entity.cpp \
    CountExpiringTimer.cpp \
    MeleeWeapon.cpp \
    Spear.cpp \
    PlayerControlledMoveBehavior.cpp \
    MoveRelativeToSelf.cpp \
    MoveRelativeToScreen.cpp

HEADERS  += \
    Map.h \
    Game.h \
    Grid.h \
    Node.h \
    Sprite.h \
    Terrain.h \
    Edge.h \
    Graph.h \
    PathGrid.h \
    PathingMap.h \
    Tree.h \
    Tests/TestPathGrid.h \
    Tests/TestPathingMap.h \
    Entity.h \
    CountExpiringTimer.h \
    MeleeWeapon.h \
    Spear.h \
    PlayerControlledMoveBehavior.h \
    MoveRelativeToSelf.h \
    MoveRelativeToScreen.h

RESOURCES += \
    res.qrc
