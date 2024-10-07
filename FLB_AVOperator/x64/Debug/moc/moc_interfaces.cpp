/****************************************************************************
** Meta object code from reading C++ file 'interfaces.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../interfaces.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interfaces.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ITimeObserver_t {
    QByteArrayData data[4];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ITimeObserver_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ITimeObserver_t qt_meta_stringdata_ITimeObserver = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ITimeObserver"
QT_MOC_LITERAL(1, 14, 15), // "onSecondChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17) // "onDurationChanged"

    },
    "ITimeObserver\0onSecondChanged\0\0"
    "onDurationChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ITimeObserver[] = {

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
       1,    1,   24,    2, 0x09 /* Protected */,
       3,    1,   27,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

       0        // eod
};

void ITimeObserver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ITimeObserver *_t = static_cast<ITimeObserver *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onSecondChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->onDurationChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ITimeObserver::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ITimeObserver.data,
      qt_meta_data_ITimeObserver,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ITimeObserver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ITimeObserver::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ITimeObserver.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ITimeObserver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_IAudioFrameProcessor_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IAudioFrameProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IAudioFrameProcessor_t qt_meta_stringdata_IAudioFrameProcessor = {
    {
QT_MOC_LITERAL(0, 0, 20) // "IAudioFrameProcessor"

    },
    "IAudioFrameProcessor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IAudioFrameProcessor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void IAudioFrameProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject IAudioFrameProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IAudioFrameProcessor.data,
      qt_meta_data_IAudioFrameProcessor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IAudioFrameProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IAudioFrameProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IAudioFrameProcessor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IAudioFrameProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_IVideoFrameProcessor_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IVideoFrameProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IVideoFrameProcessor_t qt_meta_stringdata_IVideoFrameProcessor = {
    {
QT_MOC_LITERAL(0, 0, 20) // "IVideoFrameProcessor"

    },
    "IVideoFrameProcessor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IVideoFrameProcessor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void IVideoFrameProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject IVideoFrameProcessor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IVideoFrameProcessor.data,
      qt_meta_data_IVideoFrameProcessor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IVideoFrameProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IVideoFrameProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IVideoFrameProcessor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IVideoFrameProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_IAVPathManager_t {
    QByteArrayData data[5];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IAVPathManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IAVPathManager_t qt_meta_stringdata_IAVPathManager = {
    {
QT_MOC_LITERAL(0, 0, 14), // "IAVPathManager"
QT_MOC_LITERAL(1, 15, 5), // "newfp"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 10), // "importPath"
QT_MOC_LITERAL(4, 33, 7) // "delFile"

    },
    "IAVPathManager\0newfp\0\0importPath\0"
    "delFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IAVPathManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   32,    2, 0x0a /* Public */,
       4,    1,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool, QMetaType::Int,    2,

       0        // eod
};

void IAVPathManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IAVPathManager *_t = static_cast<IAVPathManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newfp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->importPath(); break;
        case 2: { bool _r = _t->delFile((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (IAVPathManager::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IAVPathManager::newfp)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject IAVPathManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IAVPathManager.data,
      qt_meta_data_IAVPathManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IAVPathManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IAVPathManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IAVPathManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IAVPathManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void IAVPathManager::newfp(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_IFAVStream_t {
    QByteArrayData data[5];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IFAVStream_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IFAVStream_t qt_meta_stringdata_IFAVStream = {
    {
QT_MOC_LITERAL(0, 0, 10), // "IFAVStream"
QT_MOC_LITERAL(1, 11, 12), // "startChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 10), // "endChanged"
QT_MOC_LITERAL(4, 36, 11) // "needRefresh"

    },
    "IFAVStream\0startChanged\0\0endChanged\0"
    "needRefresh"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IFAVStream[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       3,    1,   32,    2, 0x06 /* Public */,
       4,    0,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,

       0        // eod
};

void IFAVStream::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IFAVStream *_t = static_cast<IFAVStream *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->endChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->needRefresh(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (IFAVStream::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IFAVStream::startChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (IFAVStream::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IFAVStream::endChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (IFAVStream::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IFAVStream::needRefresh)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject IFAVStream::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IFAVStream.data,
      qt_meta_data_IFAVStream,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IFAVStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IFAVStream::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IFAVStream.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IFAVStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void IFAVStream::startChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IFAVStream::endChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IFAVStream::needRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_IAudioStream_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IAudioStream_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IAudioStream_t qt_meta_stringdata_IAudioStream = {
    {
QT_MOC_LITERAL(0, 0, 12) // "IAudioStream"

    },
    "IAudioStream"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IAudioStream[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void IAudioStream::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject IAudioStream::staticMetaObject = {
    { &IFAVStream::staticMetaObject, qt_meta_stringdata_IAudioStream.data,
      qt_meta_data_IAudioStream,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IAudioStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IAudioStream::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IAudioStream.stringdata0))
        return static_cast<void*>(this);
    return IFAVStream::qt_metacast(_clname);
}

int IAudioStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IFAVStream::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_IVideoStream_t {
    QByteArrayData data[11];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IVideoStream_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IVideoStream_t qt_meta_stringdata_IVideoStream = {
    {
QT_MOC_LITERAL(0, 0, 12), // "IVideoStream"
QT_MOC_LITERAL(1, 13, 16), // "StreamWillAdjust"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 24), // "StreamAdjustWillFinished"
QT_MOC_LITERAL(4, 56, 20), // "streamWillAdjustThis"
QT_MOC_LITERAL(5, 77, 13), // "IVideoStream*"
QT_MOC_LITERAL(6, 91, 17), // "onRectPropChanged"
QT_MOC_LITERAL(7, 109, 14), // "onBlockPressed"
QT_MOC_LITERAL(8, 124, 20), // "initStreamRectWidget"
QT_MOC_LITERAL(9, 145, 12), // "onWillAdjust"
QT_MOC_LITERAL(10, 158, 13) // "updateVersion"

    },
    "IVideoStream\0StreamWillAdjust\0\0"
    "StreamAdjustWillFinished\0streamWillAdjustThis\0"
    "IVideoStream*\0onRectPropChanged\0"
    "onBlockPressed\0initStreamRectWidget\0"
    "onWillAdjust\0updateVersion"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IVideoStream[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   59,    2, 0x09 /* Protected */,
       7,    0,   62,    2, 0x09 /* Protected */,
       8,    0,   63,    2, 0x09 /* Protected */,
       9,    0,   64,    2, 0x09 /* Protected */,
      10,    0,   65,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QRectF,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IVideoStream::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IVideoStream *_t = static_cast<IVideoStream *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->StreamWillAdjust(); break;
        case 1: _t->StreamAdjustWillFinished(); break;
        case 2: _t->streamWillAdjustThis((*reinterpret_cast< IVideoStream*(*)>(_a[1]))); break;
        case 3: _t->onRectPropChanged((*reinterpret_cast< const QRectF(*)>(_a[1]))); break;
        case 4: _t->onBlockPressed(); break;
        case 5: _t->initStreamRectWidget(); break;
        case 6: _t->onWillAdjust(); break;
        case 7: _t->updateVersion(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< IVideoStream* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (IVideoStream::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IVideoStream::StreamWillAdjust)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (IVideoStream::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IVideoStream::StreamAdjustWillFinished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (IVideoStream::*_t)(IVideoStream * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IVideoStream::streamWillAdjustThis)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject IVideoStream::staticMetaObject = {
    { &IFAVStream::staticMetaObject, qt_meta_stringdata_IVideoStream.data,
      qt_meta_data_IVideoStream,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *IVideoStream::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IVideoStream::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IVideoStream.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "std::enable_shared_from_this<IVideoStream>"))
        return static_cast< std::enable_shared_from_this<IVideoStream>*>(this);
    return IFAVStream::qt_metacast(_clname);
}

int IVideoStream::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IFAVStream::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void IVideoStream::StreamWillAdjust()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void IVideoStream::StreamAdjustWillFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void IVideoStream::streamWillAdjustThis(IVideoStream * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
