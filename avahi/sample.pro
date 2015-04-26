TARGET = sample
TEMPLATE = app
DESTDIR = target
OBJECTS_DIR = .build
MOC_DIR = .build
QT += core widgets qml quick
CONFIG += link_pkgconfig
SOURCES = main.cpp manager.cpp avahiserviceadvertiser.cpp
HEADERS = manager.h avahiserviceadvertiser.h
PKGCONFIG += avahi-client
QMAKE_CXXFLAGS += -std=c++0x
RESOURCES += sample.qrc
