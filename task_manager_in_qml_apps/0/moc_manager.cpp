/****************************************************************************
** Meta object code from reading C++ file 'manager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "manager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'manager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Manager_t {
    QByteArrayData data[13];
    char stringdata[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Manager_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Manager_t qt_meta_stringdata_Manager = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Manager"
QT_MOC_LITERAL(1, 8, 15), // "onSlowOperation"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 22), // "onLongOperationsChain0"
QT_MOC_LITERAL(4, 48, 22), // "onLongOperationsChain1"
QT_MOC_LITERAL(5, 71, 22), // "onLongOperationsChain2"
QT_MOC_LITERAL(6, 94, 22), // "onLongOperationsChain3"
QT_MOC_LITERAL(7, 117, 22), // "onLongOperationsChain4"
QT_MOC_LITERAL(8, 140, 22), // "onLongOperationsChain5"
QT_MOC_LITERAL(9, 163, 22), // "onLongOperationsChain6"
QT_MOC_LITERAL(10, 186, 22), // "onLongOperationsChain7"
QT_MOC_LITERAL(11, 209, 22), // "onLongOperationsChain8"
QT_MOC_LITERAL(12, 232, 22) // "onLongOperationsChain9"

    },
    "Manager\0onSlowOperation\0\0"
    "onLongOperationsChain0\0onLongOperationsChain1\0"
    "onLongOperationsChain2\0onLongOperationsChain3\0"
    "onLongOperationsChain4\0onLongOperationsChain5\0"
    "onLongOperationsChain6\0onLongOperationsChain7\0"
    "onLongOperationsChain8\0onLongOperationsChain9"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Manager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    0,   74,    2, 0x0a /* Public */,
       8,    0,   75,    2, 0x0a /* Public */,
       9,    0,   76,    2, 0x0a /* Public */,
      10,    0,   77,    2, 0x0a /* Public */,
      11,    0,   78,    2, 0x0a /* Public */,
      12,    0,   79,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Manager *_t = static_cast<Manager *>(_o);
        switch (_id) {
        case 0: _t->onSlowOperation(); break;
        case 1: _t->onLongOperationsChain0(); break;
        case 2: _t->onLongOperationsChain1(); break;
        case 3: _t->onLongOperationsChain2(); break;
        case 4: _t->onLongOperationsChain3(); break;
        case 5: _t->onLongOperationsChain4(); break;
        case 6: _t->onLongOperationsChain5(); break;
        case 7: _t->onLongOperationsChain6(); break;
        case 8: _t->onLongOperationsChain7(); break;
        case 9: _t->onLongOperationsChain8(); break;
        case 10: _t->onLongOperationsChain9(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Manager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Manager.data,
      qt_meta_data_Manager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Manager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Manager.stringdata))
        return static_cast<void*>(const_cast< Manager*>(this));
    return QObject::qt_metacast(_clname);
}

int Manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
