QT += core gui widgets

TARGET = ClickerRPG
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/player.cpp \
    src/enemy.cpp

HEADERS += \
    src/mainwindow.h \
    src/player.h \
    src/enemy.h

DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    ../mapa1/green_slime.png \
    ../mapa1/jelly.png \
    ../mapa1/king_slime.png \
    ../mapa1/red_slime.png \
    ../mapa1/slime.png
