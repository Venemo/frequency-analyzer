TEMPLATE = app
TARGET = frequency-analyzer

QT += qml quick widgets multimedia
CONFIG += c++11

RESOURCES += \
    qml/desktop-qml.qrc

OTHER_FILES += \
    qml/desktop/main.qml

HEADERS += \
    audiosampler.h \
    waterfallitem.h \
    dft/dft.h \
    dft/radix2fft.h

SOURCES += main.cpp \
    audiosampler.cpp \
    waterfallitem.cpp \
    dft/dft.cpp \
    dft/radix2fft.cpp

linux-g++ {
    message("You are compiling frequency-analyzer under Linux g++")
    QMAKE_CXXFLAGS += -O3 -fvisibility=hidden -fvisibility-inlines-hidden -fPIC -finline-functions -Werror
    QMAKE_LFLAGS += -O3
}
