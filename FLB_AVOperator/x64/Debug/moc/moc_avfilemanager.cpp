/****************************************************************************
** Meta object code from reading C++ file 'avfilemanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../avfilemanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'avfilemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AVFileManager_t {
    QByteArrayData data[4];
    char stringdata0[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AVFileManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AVFileManager_t qt_meta_stringdata_AVFileManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AVFileManager"
QT_MOC_LITERAL(1, 14, 10), // "importPath"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7) // "delFile"

    },
    "AVFileManager\0importPath\0\0delFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AVFileManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    1,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Int,    2,

       0        // eod
};

void AVFileManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVFileManager *_t = static_cast<AVFileManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->importPath(); break;
        case 1: { bool _r = _t->delFile((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject AVFileManager::staticMetaObject = {
    { &IAVPathManager::staticMetaObject, qt_meta_stringdata_AVFileManager.data,
      qt_meta_data_AVFileManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AVFileManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AVFileManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AVFileManager.stringdata0))
        return static_cast<void*>(this);
    return IAVPathManager::qt_metacast(_clname);
}

int AVFileManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IAVPathManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
struct qt_meta_stringdata_AVUrlManager_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AVUrlManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AVUrlManager_t qt_meta_stringdata_AVUrlManager = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AVUrlManager"
QT_MOC_LITERAL(1, 13, 10), // "importPath"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7) // "delFile"

    },
    "AVUrlManager\0importPath\0\0delFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AVUrlManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    1,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Int,    2,

       0        // eod
};

void AVUrlManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AVUrlManager *_t = static_cast<AVUrlManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->importPath(); break;
        case 1: { bool _r = _t->delFile((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject AVUrlManager::staticMetaObject = {
    { &IAVPathManager::staticMetaObject, qt_meta_stringdata_AVUrlManager.data,
      qt_meta_data_AVUrlManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AVUrlManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AVUrlManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AVUrlManager.stringdata0))
        return static_cast<void*>(this);
    return IAVPathManager::qt_metacast(_clname);
}

int AVUrlManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IAVPathManager::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
