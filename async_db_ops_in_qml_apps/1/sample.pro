TARGET = sample
TEMPLATE = app
#QT += core gui widgets qml quick sql concurrent xml xmlpatterns multimedia websockets dbus serialport
QT += core gui widgets qml quick concurrent
SOURCES = main.cpp manager.cpp facade.cpp
HEADERS = manager.h facade.h

