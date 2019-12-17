/****************************************************************************
** Meta object code from reading C++ file 'fsc_mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../fsc_mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fsc_mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FSC_MainWindow_t {
    QByteArrayData data[54];
    char stringdata0[1299];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FSC_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FSC_MainWindow_t qt_meta_stringdata_FSC_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 14), // "FSC_MainWindow"
QT_MOC_LITERAL(1, 15, 15), // "skt_connect_suc"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 1), // "i"
QT_MOC_LITERAL(4, 34, 15), // "skt_connect_dis"
QT_MOC_LITERAL(5, 50, 9), // "skt_error"
QT_MOC_LITERAL(6, 60, 8), // "skt_read"
QT_MOC_LITERAL(7, 69, 7), // "startUp"
QT_MOC_LITERAL(8, 77, 8), // "mainLoop"
QT_MOC_LITERAL(9, 86, 8), // "startCal"
QT_MOC_LITERAL(10, 95, 9), // "startSTFM"
QT_MOC_LITERAL(11, 105, 18), // "startSocketConnect"
QT_MOC_LITERAL(12, 124, 25), // "on_tbnSysDevCheck_clicked"
QT_MOC_LITERAL(13, 150, 28), // "on_tbnManualCheckDev_clicked"
QT_MOC_LITERAL(14, 179, 42), // "on_comboBox_PlotSenSel_curren..."
QT_MOC_LITERAL(15, 222, 4), // "arg1"
QT_MOC_LITERAL(16, 227, 46), // "on_comboBox_SensorTypeName_cu..."
QT_MOC_LITERAL(17, 274, 5), // "index"
QT_MOC_LITERAL(18, 280, 30), // "on_lineEdit_setPWM_textChanged"
QT_MOC_LITERAL(19, 311, 35), // "on_lineEdit_setFlowRate_textC..."
QT_MOC_LITERAL(20, 347, 23), // "on_tbnScaleZero_clicked"
QT_MOC_LITERAL(21, 371, 22), // "on_tbnCalStart_clicked"
QT_MOC_LITERAL(22, 394, 28), // "on_tbnCalTermination_clicked"
QT_MOC_LITERAL(23, 423, 23), // "on_tbnPoltClear_clicked"
QT_MOC_LITERAL(24, 447, 33), // "on_pushButton_debugReturn_cli..."
QT_MOC_LITERAL(25, 481, 19), // "buttonDebug_clicked"
QT_MOC_LITERAL(26, 501, 30), // "on_pushButton_debugOff_clicked"
QT_MOC_LITERAL(27, 532, 25), // "on_tbnCalStepNext_clicked"
QT_MOC_LITERAL(28, 558, 30), // "on_tbnModifyFMTypePara_clicked"
QT_MOC_LITERAL(29, 589, 24), // "on_tbnCalStepPre_clicked"
QT_MOC_LITERAL(30, 614, 22), // "on_tbnCalPause_clicked"
QT_MOC_LITERAL(31, 637, 22), // "on_tbnVOutOpen_clicked"
QT_MOC_LITERAL(32, 660, 29), // "on_tbnVForwardIn1Open_clicked"
QT_MOC_LITERAL(33, 690, 29), // "on_tbnVForwardIn2Open_clicked"
QT_MOC_LITERAL(34, 720, 29), // "on_tbnVReverseIn1Open_clicked"
QT_MOC_LITERAL(35, 750, 29), // "on_tbnVReverseIn2Open_clicked"
QT_MOC_LITERAL(36, 780, 28), // "on_tbnPump1ForwardOn_clicked"
QT_MOC_LITERAL(37, 809, 28), // "on_tbnPump1ReverseOn_clicked"
QT_MOC_LITERAL(38, 838, 23), // "on_tbnVOutClose_clicked"
QT_MOC_LITERAL(39, 862, 30), // "on_tbnVForwardIn1Close_clicked"
QT_MOC_LITERAL(40, 893, 30), // "on_tbnVForwardIn2Close_clicked"
QT_MOC_LITERAL(41, 924, 30), // "on_tbnVReverseIn1Close_clicked"
QT_MOC_LITERAL(42, 955, 30), // "on_tbnVReverseIn2Close_clicked"
QT_MOC_LITERAL(43, 986, 29), // "on_tbnPump1ForwardOff_clicked"
QT_MOC_LITERAL(44, 1016, 29), // "on_tbnPump1ReverseOff_clicked"
QT_MOC_LITERAL(45, 1046, 21), // "on_tbnParaAdd_clicked"
QT_MOC_LITERAL(46, 1068, 23), // "on_tbnParaErase_clicked"
QT_MOC_LITERAL(47, 1092, 28), // "on_tbnPump2ForwardOn_clicked"
QT_MOC_LITERAL(48, 1121, 28), // "on_tbnPump2ReverseOn_clicked"
QT_MOC_LITERAL(49, 1150, 29), // "on_tbnPump2ForwardOff_clicked"
QT_MOC_LITERAL(50, 1180, 29), // "on_tbnPump2ReverseOff_clicked"
QT_MOC_LITERAL(51, 1210, 34), // "on_radioButton_setFlowRate_cl..."
QT_MOC_LITERAL(52, 1245, 29), // "on_radioButton_setPWM_clicked"
QT_MOC_LITERAL(53, 1275, 23) // "on_tbnCalManual_clicked"

    },
    "FSC_MainWindow\0skt_connect_suc\0\0i\0"
    "skt_connect_dis\0skt_error\0skt_read\0"
    "startUp\0mainLoop\0startCal\0startSTFM\0"
    "startSocketConnect\0on_tbnSysDevCheck_clicked\0"
    "on_tbnManualCheckDev_clicked\0"
    "on_comboBox_PlotSenSel_currentIndexChanged\0"
    "arg1\0on_comboBox_SensorTypeName_currentIndexChanged\0"
    "index\0on_lineEdit_setPWM_textChanged\0"
    "on_lineEdit_setFlowRate_textChanged\0"
    "on_tbnScaleZero_clicked\0on_tbnCalStart_clicked\0"
    "on_tbnCalTermination_clicked\0"
    "on_tbnPoltClear_clicked\0"
    "on_pushButton_debugReturn_clicked\0"
    "buttonDebug_clicked\0on_pushButton_debugOff_clicked\0"
    "on_tbnCalStepNext_clicked\0"
    "on_tbnModifyFMTypePara_clicked\0"
    "on_tbnCalStepPre_clicked\0"
    "on_tbnCalPause_clicked\0on_tbnVOutOpen_clicked\0"
    "on_tbnVForwardIn1Open_clicked\0"
    "on_tbnVForwardIn2Open_clicked\0"
    "on_tbnVReverseIn1Open_clicked\0"
    "on_tbnVReverseIn2Open_clicked\0"
    "on_tbnPump1ForwardOn_clicked\0"
    "on_tbnPump1ReverseOn_clicked\0"
    "on_tbnVOutClose_clicked\0"
    "on_tbnVForwardIn1Close_clicked\0"
    "on_tbnVForwardIn2Close_clicked\0"
    "on_tbnVReverseIn1Close_clicked\0"
    "on_tbnVReverseIn2Close_clicked\0"
    "on_tbnPump1ForwardOff_clicked\0"
    "on_tbnPump1ReverseOff_clicked\0"
    "on_tbnParaAdd_clicked\0on_tbnParaErase_clicked\0"
    "on_tbnPump2ForwardOn_clicked\0"
    "on_tbnPump2ReverseOn_clicked\0"
    "on_tbnPump2ForwardOff_clicked\0"
    "on_tbnPump2ReverseOff_clicked\0"
    "on_radioButton_setFlowRate_clicked\0"
    "on_radioButton_setPWM_clicked\0"
    "on_tbnCalManual_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FSC_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  259,    2, 0x08 /* Private */,
       4,    1,  262,    2, 0x08 /* Private */,
       5,    1,  265,    2, 0x08 /* Private */,
       6,    1,  268,    2, 0x08 /* Private */,
       7,    0,  271,    2, 0x08 /* Private */,
       8,    0,  272,    2, 0x08 /* Private */,
       9,    0,  273,    2, 0x08 /* Private */,
      10,    0,  274,    2, 0x08 /* Private */,
      11,    1,  275,    2, 0x08 /* Private */,
      12,    0,  278,    2, 0x08 /* Private */,
      13,    0,  279,    2, 0x08 /* Private */,
      14,    1,  280,    2, 0x08 /* Private */,
      16,    1,  283,    2, 0x08 /* Private */,
      18,    1,  286,    2, 0x08 /* Private */,
      19,    1,  289,    2, 0x08 /* Private */,
      20,    0,  292,    2, 0x08 /* Private */,
      21,    0,  293,    2, 0x08 /* Private */,
      22,    0,  294,    2, 0x08 /* Private */,
      23,    0,  295,    2, 0x08 /* Private */,
      24,    0,  296,    2, 0x08 /* Private */,
      25,    1,  297,    2, 0x08 /* Private */,
      26,    0,  300,    2, 0x08 /* Private */,
      27,    0,  301,    2, 0x08 /* Private */,
      28,    0,  302,    2, 0x08 /* Private */,
      29,    0,  303,    2, 0x08 /* Private */,
      30,    0,  304,    2, 0x08 /* Private */,
      31,    0,  305,    2, 0x08 /* Private */,
      32,    0,  306,    2, 0x08 /* Private */,
      33,    0,  307,    2, 0x08 /* Private */,
      34,    0,  308,    2, 0x08 /* Private */,
      35,    0,  309,    2, 0x08 /* Private */,
      36,    0,  310,    2, 0x08 /* Private */,
      37,    0,  311,    2, 0x08 /* Private */,
      38,    0,  312,    2, 0x08 /* Private */,
      39,    0,  313,    2, 0x08 /* Private */,
      40,    0,  314,    2, 0x08 /* Private */,
      41,    0,  315,    2, 0x08 /* Private */,
      42,    0,  316,    2, 0x08 /* Private */,
      43,    0,  317,    2, 0x08 /* Private */,
      44,    0,  318,    2, 0x08 /* Private */,
      45,    0,  319,    2, 0x08 /* Private */,
      46,    0,  320,    2, 0x08 /* Private */,
      47,    0,  321,    2, 0x08 /* Private */,
      48,    0,  322,    2, 0x08 /* Private */,
      49,    0,  323,    2, 0x08 /* Private */,
      50,    0,  324,    2, 0x08 /* Private */,
      51,    0,  325,    2, 0x08 /* Private */,
      52,    0,  326,    2, 0x08 /* Private */,
      53,    0,  327,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FSC_MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FSC_MainWindow *_t = static_cast<FSC_MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->skt_connect_suc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->skt_connect_dis((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->skt_error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->skt_read((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->startUp(); break;
        case 5: _t->mainLoop(); break;
        case 6: _t->startCal(); break;
        case 7: _t->startSTFM(); break;
        case 8: _t->startSocketConnect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_tbnSysDevCheck_clicked(); break;
        case 10: _t->on_tbnManualCheckDev_clicked(); break;
        case 11: _t->on_comboBox_PlotSenSel_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->on_comboBox_SensorTypeName_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_lineEdit_setPWM_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->on_lineEdit_setFlowRate_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: _t->on_tbnScaleZero_clicked(); break;
        case 16: _t->on_tbnCalStart_clicked(); break;
        case 17: _t->on_tbnCalTermination_clicked(); break;
        case 18: _t->on_tbnPoltClear_clicked(); break;
        case 19: _t->on_pushButton_debugReturn_clicked(); break;
        case 20: _t->buttonDebug_clicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->on_pushButton_debugOff_clicked(); break;
        case 22: _t->on_tbnCalStepNext_clicked(); break;
        case 23: _t->on_tbnModifyFMTypePara_clicked(); break;
        case 24: _t->on_tbnCalStepPre_clicked(); break;
        case 25: _t->on_tbnCalPause_clicked(); break;
        case 26: _t->on_tbnVOutOpen_clicked(); break;
        case 27: _t->on_tbnVForwardIn1Open_clicked(); break;
        case 28: _t->on_tbnVForwardIn2Open_clicked(); break;
        case 29: _t->on_tbnVReverseIn1Open_clicked(); break;
        case 30: _t->on_tbnVReverseIn2Open_clicked(); break;
        case 31: _t->on_tbnPump1ForwardOn_clicked(); break;
        case 32: _t->on_tbnPump1ReverseOn_clicked(); break;
        case 33: _t->on_tbnVOutClose_clicked(); break;
        case 34: _t->on_tbnVForwardIn1Close_clicked(); break;
        case 35: _t->on_tbnVForwardIn2Close_clicked(); break;
        case 36: _t->on_tbnVReverseIn1Close_clicked(); break;
        case 37: _t->on_tbnVReverseIn2Close_clicked(); break;
        case 38: _t->on_tbnPump1ForwardOff_clicked(); break;
        case 39: _t->on_tbnPump1ReverseOff_clicked(); break;
        case 40: _t->on_tbnParaAdd_clicked(); break;
        case 41: _t->on_tbnParaErase_clicked(); break;
        case 42: _t->on_tbnPump2ForwardOn_clicked(); break;
        case 43: _t->on_tbnPump2ReverseOn_clicked(); break;
        case 44: _t->on_tbnPump2ForwardOff_clicked(); break;
        case 45: _t->on_tbnPump2ReverseOff_clicked(); break;
        case 46: _t->on_radioButton_setFlowRate_clicked(); break;
        case 47: _t->on_radioButton_setPWM_clicked(); break;
        case 48: _t->on_tbnCalManual_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject FSC_MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FSC_MainWindow.data,
      qt_meta_data_FSC_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FSC_MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FSC_MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FSC_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FSC_MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 49)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 49;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
