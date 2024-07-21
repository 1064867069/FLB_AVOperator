/****************************************************************************
** Meta object code from reading C++ file 'timeobservers.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../timeobservers.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'timeobservers.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SliderController_t {
    QByteArrayData data[8];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SliderController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SliderController_t qt_meta_stringdata_SliderController = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SliderController"
QT_MOC_LITERAL(1, 17, 8), // "seekProp"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "onSecondChanged"
QT_MOC_LITERAL(4, 43, 17), // "onDurationChanged"
QT_MOC_LITERAL(5, 61, 15), // "onSliderPressed"
QT_MOC_LITERAL(6, 77, 16), // "onSliderReleased"
QT_MOC_LITERAL(7, 94, 11) // "resetSlider"

    },
    "SliderController\0seekProp\0\0onSecondChanged\0"
    "onDurationChanged\0onSliderPressed\0"
    "onSliderReleased\0resetSlider"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SliderController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   47,    2, 0x09 /* Protected */,
       4,    1,   50,    2, 0x09 /* Protected */,
       5,    0,   53,    2, 0x09 /* Protected */,
       6,    0,   54,    2, 0x09 /* Protected */,
       7,    0,   55,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SliderController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SliderController *_t = static_cast<SliderController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->seekProp((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->onSecondChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->onDurationChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->onSliderPressed(); break;
        case 4: _t->onSliderReleased(); break;
        case 5: _t->resetSlider(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SliderController::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SliderController::seekProp)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject SliderController::staticMetaObject = {
    { &ITimeObserver::staticMetaObject, qt_meta_stringdata_SliderController.data,
      qt_meta_data_SliderController,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SliderController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SliderController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SliderController.stringdata0))
        return static_cast<void*>(this);
    return ITimeObserver::qt_metacast(_clname);
}

int SliderController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ITimeObserver::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SliderController::seekProp(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_LabelTimeStampObserver_t {
    QByteArrayData data[5];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabelTimeStampObserver_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabelTimeStampObserver_t qt_meta_stringdata_LabelTimeStampObserver = {
    {
QT_MOC_LITERAL(0, 0, 22), // "LabelTimeStampObserver"
QT_MOC_LITERAL(1, 23, 10), // "resetStamp"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "onSecondChanged"
QT_MOC_LITERAL(4, 51, 17) // "onDurationChanged"

    },
    "LabelTimeStampObserver\0resetStamp\0\0"
    "onSecondChanged\0onDurationChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabelTimeStampObserver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x09 /* Protected */,
       4,    1,   33,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void LabelTimeStampObserver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LabelTimeStampObserver *_t = static_cast<LabelTimeStampObserver *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->resetStamp(); break;
        case 1: _t->onSecondChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->onDurationChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject LabelTimeStampObserver::staticMetaObject = {
    { &ITimeObserver::staticMetaObject, qt_meta_stringdata_LabelTimeStampObserver.data,
      qt_meta_data_LabelTimeStampObserver,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *LabelTimeStampObserver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabelTimeStampObserver::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabelTimeStampObserver.stringdata0))
        return static_cast<void*>(this);
    return ITimeObserver::qt_metacast(_clname);
}

int LabelTimeStampObserver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ITimeObserver::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
