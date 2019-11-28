/****************************************************************************
** Meta object code from reading C++ file 'FCMainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../FCMainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FCMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FCMainWindow_t {
    QByteArrayData data[19];
    char stringdata0[234];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FCMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FCMainWindow_t qt_meta_stringdata_FCMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FCMainWindow"
QT_MOC_LITERAL(1, 13, 15), // "FindNextComport"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "ConnectComport"
QT_MOC_LITERAL(4, 45, 11), // "ReadRecData"
QT_MOC_LITERAL(5, 57, 11), // "TimerUpdate"
QT_MOC_LITERAL(6, 69, 18), // "ConnectNextComport"
QT_MOC_LITERAL(7, 88, 15), // "ReadNextRecData"
QT_MOC_LITERAL(8, 104, 11), // "DutyCycleUp"
QT_MOC_LITERAL(9, 116, 13), // "DutyCycleDown"
QT_MOC_LITERAL(10, 130, 6), // "Switch"
QT_MOC_LITERAL(11, 137, 7), // "Measure"
QT_MOC_LITERAL(12, 145, 10), // "ClearWaves"
QT_MOC_LITERAL(13, 156, 5), // "Reset"
QT_MOC_LITERAL(14, 162, 10), // "SetAddress"
QT_MOC_LITERAL(15, 173, 10), // "RecAddress"
QT_MOC_LITERAL(16, 184, 12), // "QVector<int>"
QT_MOC_LITERAL(17, 197, 15), // "FlowCalibration"
QT_MOC_LITERAL(18, 213, 20) // "FlowSpeedCalibration"

    },
    "FCMainWindow\0FindNextComport\0\0"
    "ConnectComport\0ReadRecData\0TimerUpdate\0"
    "ConnectNextComport\0ReadNextRecData\0"
    "DutyCycleUp\0DutyCycleDown\0Switch\0"
    "Measure\0ClearWaves\0Reset\0SetAddress\0"
    "RecAddress\0QVector<int>\0FlowCalibration\0"
    "FlowSpeedCalibration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FCMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    1,  107,    2, 0x08 /* Private */,
      17,    0,  110,    2, 0x08 /* Private */,
      18,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

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
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FCMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FCMainWindow *_t = static_cast<FCMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->FindNextComport(); break;
        case 1: _t->ConnectComport(); break;
        case 2: _t->ReadRecData(); break;
        case 3: _t->TimerUpdate(); break;
        case 4: _t->ConnectNextComport(); break;
        case 5: _t->ReadNextRecData(); break;
        case 6: _t->DutyCycleUp(); break;
        case 7: _t->DutyCycleDown(); break;
        case 8: _t->Switch(); break;
        case 9: _t->Measure(); break;
        case 10: _t->ClearWaves(); break;
        case 11: _t->Reset(); break;
        case 12: _t->SetAddress(); break;
        case 13: _t->RecAddress((*reinterpret_cast< QVector<int>(*)>(_a[1]))); break;
        case 14: _t->FlowCalibration(); break;
        case 15: _t->FlowSpeedCalibration(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FCMainWindow::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FCMainWindow::FindNextComport)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject FCMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FCMainWindow.data,
      qt_meta_data_FCMainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FCMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FCMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FCMainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FCMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void FCMainWindow::FindNextComport()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
