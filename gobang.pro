QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game_gobang.cpp \
    game_issave.cpp \
    game_othello.cpp \
    game_piece.cpp \
    game_savedgame.cpp \
    game_test.cpp \
    game_widget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Common.h \
    game_gobang.h \
    game_issave.h \
    game_othello.h \
    game_piece.h \
    game_savedgame.h \
    game_test.h \
    game_widget.h \
    mainwindow.h

FORMS += \
    game_gobang.ui \
    game_issave.ui \
    game_othello.ui \
    game_piece.ui \
    game_savedgame.ui \
    game_widget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../棋盘.png \
    GameOthello.qss

INCLUDEPATH += E:\boost_1_79_0
LIBS += "-LE:\boost_1_79_0\stage\lib"
