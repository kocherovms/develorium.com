TARGET = sample
TEMPLATE = app
DESTDIR = target
OBJECTS_DIR = .build
MOC_DIR = .build
QT += core widgets qml quick concurrent
SOURCES = main.cpp manager.cpp taskmanager.cpp
HEADERS = manager.h taskmanager.h
QMAKE_CXXFLAGS += -std=c++0x
RESOURCES += sample.qrc
