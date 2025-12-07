QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bala.cpp \
    combate.cpp \
    enemigo.cpp \
    jugador.cpp \
    jugadorcombate.cpp \
    main.cpp \
    mainwindow.cpp \
    obstaculo.cpp \
    personaje.cpp \
    personas.cpp

HEADERS += \
    bala.h \
    combate.h \
    enemigo.h \
    jugador.h \
    jugadorcombate.h \
    mainwindow.h \
    obstaculo.h \
    personaje.h \
    personas.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Texxturas/spritespersonajeprincipal.png

RESOURCES += \
    recursos.qrc
