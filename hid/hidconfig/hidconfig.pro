TEMPLATE = app
TARGET = hidconfig
CONFIG += qt
include(../build.pro)

CONFIG(debug,debug|release) {
DESTDIR=debug
} else {
DESTDIR=release
}

QT += xml xmlpatterns

DEFINES += QT_XML_LIB

INCLUDEPATH +=  ./generatedfiles
CONFIG(debug,debug|release) {
INCLUDEPATH +=  ./generatedfiles/debug
} else {
INCLUDEPATH +=  ./generatedfiles/release
}

win32:INCLUDEPATH += ../libusb-win32-device-bin-0.1.12.1/include
unix:INCLUDEPATH += ../libusb-1.0.2/libusb ../utility ../usbhid ../hid ../include

unix:LIBS += -L../hid/debug -lhid -L../usbhid/debug -lusbhid -L../utility/debug -lutility -L../libusb-1.0.2/libusb/.libs -l:libusb-1.0.a 

PRE_TARGETDEPS = ../hid/debug/libhid.a ../usbhid/debug/libusbhid.a ../utility/debug/libutility.a

PRECOMPILED_HEADER = stdafx.h
DEFINES += USING_PCH
QMAKE_CXXFLAGS += -include stdafx.h

DEPENDPATH += .
CONFIG(debug,debug|release) {
MOC_DIR += ./generatedfiles/debug
} else {
MOC_DIR += ./generatedfiles/release
}
CONFIG(debug,debug|release) {
OBJECTS_DIR += debug
} else {
OBJECTS_DIR += release
}
UI_DIR += ./generatedfiles
RCC_DIR += ./generatedfiles

#Include file(s)
include(hidconfig.pri) 

