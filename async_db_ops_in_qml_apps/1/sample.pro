TARGET = sample
TEMPLATE = app
DESTDIR = target
OBJECTS_DIR = .build
MOC_DIR = .build
QT += core widgets qml quick
SOURCES = main.cpp manager.cpp facade.cpp
HEADERS = manager.h facade.h
RESOURCES += sample.qrc
