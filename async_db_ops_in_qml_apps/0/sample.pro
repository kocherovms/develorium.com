TARGET = sample
TEMPLATE = app
DESTDIR = target
OBJECTS_DIR = .build
MOC_DIR = .build
QT += core widgets qml quick
SOURCES = main.cpp manager.cpp 
HEADERS = manager.h 
RESOURCES += sample.qrc

