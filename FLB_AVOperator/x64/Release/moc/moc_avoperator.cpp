/****************************************************************************
** Meta object code from reading C++ file 'avoperator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../avoperator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'avoperator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FAVFileReader_t {
    QByteArrayData data[5];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FAVFileReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FAVFileReader_t qt_meta_stringdata_FAVFileReader = {
    {
QT_MOC_LITERAL(0, 0, 13), // "FAVFileReader"
QT_MOC_LITERAL(1, 14, 21), // "durationSecondChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 9), // "decodeEnd"
QT_MOC_LITERAL(4, 47, 12) // "seekFinished"

    },
    "FAVFileReader\0durationSecondChanged\0"
    "\0decodeEnd\0seekFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FAVFileReader[] = {

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
       3,    0,   32,    2, 0x06 /* Public */,
       4,    0,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FAVFileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FAVFileReader *_t = static_cast<FAVFileReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->durationSecondChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->decodeEnd(); break;
        case 2: _t->seekFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FAVFileReader::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVFileReader::durationSecondChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FAVFileReader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVFileReader::decodeEnd)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FAVFileReader::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVFileReader::seekFinished)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject FAVFileReader::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FAVFileReader.data,
      qt_meta_data_FAVFileReader,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FAVFileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FAVFileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FAVFileReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FAVFileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void FAVFileReader::durationSecondChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FAVFileReader::decodeEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FAVFileReader::seekFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_AudioSDLPlayer_t {
    QByteArrayData data[4];
    char stringdata0[47];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioSDLPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioSDLPlayer_t qt_meta_stringdata_AudioSDLPlayer = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AudioSDLPlayer"
QT_MOC_LITERAL(1, 15, 19), // "avPlayerBindChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 10) // "FAVPlayer*"

    },
    "AudioSDLPlayer\0avPlayerBindChanged\0\0"
    "FAVPlayer*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioSDLPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

       0        // eod
};

void AudioSDLPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioSDLPlayer *_t = static_cast<AudioSDLPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->avPlayerBindChanged((*reinterpret_cast< FAVPlayer*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< FAVPlayer* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (AudioSDLPlayer::*_t)(FAVPlayer * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioSDLPlayer::avPlayerBindChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AudioSDLPlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AudioSDLPlayer.data,
      qt_meta_data_AudioSDLPlayer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *AudioSDLPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioSDLPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AudioSDLPlayer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AudioSDLPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void AudioSDLPlayer::avPlayerBindChanged(FAVPlayer * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_FAVPlayer_t {
    QByteArrayData data[12];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FAVPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FAVPlayer_t qt_meta_stringdata_FAVPlayer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "FAVPlayer"
QT_MOC_LITERAL(1, 10, 13), // "secondChanged"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 15), // "durationChanged"
QT_MOC_LITERAL(4, 41, 7), // "started"
QT_MOC_LITERAL(5, 49, 8), // "seekProp"
QT_MOC_LITERAL(6, 58, 12), // "setCurSecond"
QT_MOC_LITERAL(7, 71, 11), // "setDuration"
QT_MOC_LITERAL(8, 83, 14), // "onSeekFinished"
QT_MOC_LITERAL(9, 98, 10), // "onAudioEnd"
QT_MOC_LITERAL(10, 109, 10), // "onVideoEnd"
QT_MOC_LITERAL(11, 120, 10) // "check_stop"

    },
    "FAVPlayer\0secondChanged\0\0durationChanged\0"
    "started\0seekProp\0setCurSecond\0setDuration\0"
    "onSeekFinished\0onAudioEnd\0onVideoEnd\0"
    "check_stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FAVPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x06 /* Public */,
       4,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   71,    2, 0x0a /* Public */,
       6,    1,   74,    2, 0x08 /* Private */,
       7,    1,   77,    2, 0x08 /* Private */,
       8,    0,   80,    2, 0x08 /* Private */,
       9,    0,   81,    2, 0x08 /* Private */,
      10,    0,   82,    2, 0x08 /* Private */,
      11,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FAVPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FAVPlayer *_t = static_cast<FAVPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->secondChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->durationChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->started(); break;
        case 3: _t->seekProp((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setCurSecond((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->onSeekFinished(); break;
        case 7: _t->onAudioEnd(); break;
        case 8: _t->onVideoEnd(); break;
        case 9: _t->check_stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FAVPlayer::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVPlayer::secondChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (FAVPlayer::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVPlayer::durationChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (FAVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVPlayer::started)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject FAVPlayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FAVPlayer.data,
      qt_meta_data_FAVPlayer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FAVPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FAVPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FAVPlayer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FAVPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void FAVPlayer::secondChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FAVPlayer::durationChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FAVPlayer::started()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
