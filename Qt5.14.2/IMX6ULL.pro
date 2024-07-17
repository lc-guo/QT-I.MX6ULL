QT       += core gui svg
QT       += charts
QT       += serialport serialbus



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
    LayoutSquare.cpp \
    WidgetADI.cpp \
    adc.cpp \
    ap3216c.cpp \
    ble.cpp \
    camera.cpp \
    can.cpp \
    config.cpp \
    gpio.cpp \
    icm20608.cpp \
    main.cpp \
    mainwindow.cpp \
    music_player.cpp \
    qfi_ADI.cpp \
    recording.cpp \
    showphoto.cpp \
    sketchpad.cpp \
    uart.cpp \
    video.cpp

HEADERS += \
    LayoutSquare.h \
    WidgetADI.h \
    adc.h \
    ap3216c.h \
    ble.h \
    camera.h \
    can.h \
    config.h \
    gpio.h \
    icm20608.h \
    mainwindow.h \
    music_player.h \
    qfi_ADI.h \
    recording.h \
    showphoto.h \
    sketchpad.h \
    uart.h \
    video.h

FORMS += \
    WidgetADI.ui \
    adc.ui \
    ap3216c.ui \
    ble.ui \
    camera.ui \
    can.ui \
    config.ui \
    gpio.ui \
    icm20608.ui \
    mainwindow.ui \
    music_player.ui \
    recording.ui \
    showphoto.ui \
    sketchpad.ui \
    uart.ui \
    video.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    fonts.qrc \
    images.qrc
