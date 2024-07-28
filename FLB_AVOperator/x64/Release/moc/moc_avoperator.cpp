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
    QByteArrayData data[5];
    char stringdata0[56];
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
QT_MOC_LITERAL(3, 36, 10), // "FAVPlayer*"
QT_MOC_LITERAL(4, 47, 8) // "audioEnd"

    },
    "AudioSDLPlayer\0avPlayerBindChanged\0\0"
    "FAVPlayer*\0audioEnd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioSDLPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
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
        case 1: _t->audioEnd((*reinterpret_cast< FAVPlayer*(*)>(_a[1]))); break;
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
        case 1:
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
        {
            typedef void (AudioSDLPlayer::*_t)(FAVPlayer * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AudioSDLPlayer::audioEnd)) {
                *result = 1;
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
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AudioSDLPlayer::avPlayerBindChanged(FAVPlayer * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AudioSDLPlayer::audioEnd(FAVPlayer * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_FAVPlayer_t {
    QByteArrayData data[19];
    char stringdata0[195];
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
QT_MOC_LITERAL(5, 49, 7), // "stopped"
QT_MOC_LITERAL(6, 57, 4), // "stop"
QT_MOC_LITERAL(7, 62, 5), // "pause"
QT_MOC_LITERAL(8, 68, 8), // "playCont"
QT_MOC_LITERAL(9, 77, 8), // "seekProp"
QT_MOC_LITERAL(10, 86, 11), // "seekForward"
QT_MOC_LITERAL(11, 98, 12), // "seekBackward"
QT_MOC_LITERAL(12, 111, 12), // "setCurSecond"
QT_MOC_LITERAL(13, 124, 11), // "setDuration"
QT_MOC_LITERAL(14, 136, 14), // "onSeekFinished"
QT_MOC_LITERAL(15, 151, 10), // "onAudioEnd"
QT_MOC_LITERAL(16, 162, 10), // "FAVPlayer*"
QT_MOC_LITERAL(17, 173, 10), // "onVideoEnd"
QT_MOC_LITERAL(18, 184, 10) // "check_stop"

    },
    "FAVPlayer\0secondChanged\0\0durationChanged\0"
    "started\0stopped\0stop\0pause\0playCont\0"
    "seekProp\0seekForward\0seekBackward\0"
    "setCurSecond\0setDuration\0onSeekFinished\0"
    "onAudioEnd\0FAVPlayer*\0onVideoEnd\0"
    "check_stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FAVPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       3,    1,   97,    2, 0x06 /* Public */,
       4,    0,  100,    2, 0x06 /* Public */,
       5,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,  102,    2, 0x0a /* Public */,
       7,    0,  103,    2, 0x0a /* Public */,
       8,    0,  104,    2, 0x0a /* Public */,
       9,    1,  105,    2, 0x0a /* Public */,
      10,    0,  108,    2, 0x0a /* Public */,
      11,    0,  109,    2, 0x0a /* Public */,
      12,    1,  110,    2, 0x08 /* Private */,
      13,    1,  113,    2, 0x08 /* Private */,
      14,    0,  116,    2, 0x08 /* Private */,
      15,    1,  117,    2, 0x08 /* Private */,
      17,    0,  120,    2, 0x08 /* Private */,
      18,    0,  121,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,    2,
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
        case 3: _t->stopped(); break;
        case 4: _t->stop(); break;
        case 5: _t->pause(); break;
        case 6: _t->playCont(); break;
        case 7: _t->seekProp((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->seekForward(); break;
        case 9: _t->seekBackward(); break;
        case 10: _t->setCurSecond((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->setDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->onSeekFinished(); break;
        case 13: _t->onAudioEnd((*reinterpret_cast< FAVPlayer*(*)>(_a[1]))); break;
        case 14: _t->onVideoEnd(); break;
        case 15: _t->check_stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 13:
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
        {
            typedef void (FAVPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVPlayer::stopped)) {
                *result = 3;
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

// SIGNAL 3
void FAVPlayer::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
