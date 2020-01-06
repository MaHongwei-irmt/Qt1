/****************************************************************************
** Meta object code from reading C++ file 'dialog_fmdata.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialog_fmdata.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog_fmdata.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Dialog_fmdata_t {
    QByteArrayData data[11];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dialog_fmdata_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dialog_fmdata_t qt_meta_stringdata_Dialog_fmdata = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Dialog_fmdata"
QT_MOC_LITERAL(1, 14, 18), // "on_tbnRead_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 19), // "on_tbnWrite_clicked"
QT_MOC_LITERAL(4, 54, 26), // "on_tbnRead_SET_KF2_clicked"
QT_MOC_LITERAL(5, 81, 27), // "on_tbnWrite_SET_KF2_clicked"
QT_MOC_LITERAL(6, 109, 29), // "on_tbnRead_FLOW_RANHE_clicked"
QT_MOC_LITERAL(7, 139, 30), // "on_tbnWrite_FLOW_RANHE_clicked"
QT_MOC_LITERAL(8, 170, 27), // "on_tbnRead_ZERO_CAL_clicked"
QT_MOC_LITERAL(9, 198, 28), // "on_tbnWrite_ZERO_CAL_clicked"
QT_MOC_LITERAL(10, 227, 24) // "on_tbnFMRegister_clicked"

    },
    "Dialog_fmdata\0on_tbnRead_clicked\0\0"
    "on_tbnWrite_clicked\0on_tbnRead_SET_KF2_clicked\0"
    "on_tbnWrite_SET_KF2_clicked\0"
    "on_tbnRead_FLOW_RANHE_clicked\0"
    "on_tbnWrite_FLOW_RANHE_clicked\0"
    "on_tbnRead_ZERO_CAL_clicked\0"
    "on_tbnWrite_ZERO_CAL_clicked\0"
    "on_tbnFMRegister_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dialog_fmdata[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

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

       0        // eod
};

void Dialog_fmdata::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Dialog_fmdata *_t = static_cast<Dialog_fmdata *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_tbnRead_clicked(); break;
        case 1: _t->on_tbnWrite_clicked(); break;
        case 2: _t->on_tbnRead_SET_KF2_clicked(); break;
        case 3: _t->on_tbnWrite_SET_KF2_clicked(); break;
        case 4: _t->on_tbnRead_FLOW_RANHE_clicked(); break;
        case 5: _t->on_tbnWrite_FLOW_RANHE_clicked(); break;
        case 6: _t->on_tbnRead_ZERO_CAL_clicked(); break;
        case 7: _t->on_tbnWrite_ZERO_CAL_clicked(); break;
        case 8: _t->on_tbnFMRegister_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Dialog_fmdata::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog_fmdata.data,
      qt_meta_data_Dialog_fmdata,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Dialog_fmdata::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dialog_fmdata::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog_fmdata.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Dialog_fmdata::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
