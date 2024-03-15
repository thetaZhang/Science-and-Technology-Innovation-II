/****************************************************************************
** Meta object code from reading C++ file 'heartratesensor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../heartRateDemo/heartratesensor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'heartratesensor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HeartRateSensor_t {
    const uint offsetsAndSize[20];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_HeartRateSensor_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_HeartRateSensor_t qt_meta_stringdata_HeartRateSensor = {
    {
QT_MOC_LITERAL(0, 15), // "HeartRateSensor"
QT_MOC_LITERAL(16, 22), // "heartRateRawDataSignal"
QT_MOC_LITERAL(39, 0), // ""
QT_MOC_LITERAL(40, 18), // "std::vector<float>"
QT_MOC_LITERAL(59, 7), // "rawdata"
QT_MOC_LITERAL(67, 15), // "heartRateSignal"
QT_MOC_LITERAL(83, 9), // "heartRate"
QT_MOC_LITERAL(93, 11), // "startSample"
QT_MOC_LITERAL(105, 10), // "stopSample"
QT_MOC_LITERAL(116, 13) // "onDataTimerCb"

    },
    "HeartRateSensor\0heartRateRawDataSignal\0"
    "\0std::vector<float>\0rawdata\0heartRateSignal\0"
    "heartRate\0startSample\0stopSample\0"
    "onDataTimerCb"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HeartRateSensor[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   50,    2, 0x0a,    5 /* Public */,
       8,    0,   51,    2, 0x0a,    6 /* Public */,
       9,    0,   52,    2, 0x08,    7 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Float,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HeartRateSensor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HeartRateSensor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->heartRateRawDataSignal((*reinterpret_cast< std::add_pointer_t<std::vector<float>>>(_a[1]))); break;
        case 1: _t->heartRateSignal((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 2: _t->startSample(); break;
        case 3: _t->stopSample(); break;
        case 4: _t->onDataTimerCb(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HeartRateSensor::*)(std::vector<float> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HeartRateSensor::heartRateRawDataSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HeartRateSensor::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HeartRateSensor::heartRateSignal)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject HeartRateSensor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HeartRateSensor.offsetsAndSize,
    qt_meta_data_HeartRateSensor,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_HeartRateSensor_t
, QtPrivate::TypeAndForceComplete<HeartRateSensor, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::vector<float>, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<float, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *HeartRateSensor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HeartRateSensor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HeartRateSensor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HeartRateSensor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void HeartRateSensor::heartRateRawDataSignal(std::vector<float> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HeartRateSensor::heartRateSignal(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
