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
    QByteArrayData data[22];
    char stringdata0[220];
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
QT_MOC_LITERAL(6, 57, 4), // "seek"
QT_MOC_LITERAL(7, 62, 4), // "stop"
QT_MOC_LITERAL(8, 67, 5), // "pause"
QT_MOC_LITERAL(9, 73, 8), // "playCont"
QT_MOC_LITERAL(10, 82, 8), // "seekProp"
QT_MOC_LITERAL(11, 91, 11), // "seekForward"
QT_MOC_LITERAL(12, 103, 12), // "seekBackward"
QT_MOC_LITERAL(13, 116, 11), // "setDuration"
QT_MOC_LITERAL(14, 128, 14), // "onSeekFinished"
QT_MOC_LITERAL(15, 143, 12), // "setCurSecond"
QT_MOC_LITERAL(16, 156, 10), // "onAudioEnd"
QT_MOC_LITERAL(17, 167, 10), // "FAVPlayer*"
QT_MOC_LITERAL(18, 178, 10), // "onVideoEnd"
QT_MOC_LITERAL(19, 189, 10), // "check_stop"
QT_MOC_LITERAL(20, 200, 8), // "openPath"
QT_MOC_LITERAL(21, 209, 10) // "ReaderSPtr"

    },
    "FAVPlayer\0secondChanged\0\0durationChanged\0"
    "started\0stopped\0seek\0stop\0pause\0"
    "playCont\0seekProp\0seekForward\0"
    "seekBackward\0setDuration\0onSeekFinished\0"
    "setCurSecond\0onAudioEnd\0FAVPlayer*\0"
    "onVideoEnd\0check_stop\0openPath\0"
    "ReaderSPtr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FAVPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       3,    1,  107,    2, 0x06 /* Public */,
       4,    0,  110,    2, 0x06 /* Public */,
       5,    0,  111,    2, 0x06 /* Public */,
       6,    1,  112,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  115,    2, 0x0a /* Public */,
       8,    0,  116,    2, 0x0a /* Public */,
       9,    0,  117,    2, 0x0a /* Public */,
      10,    1,  118,    2, 0x0a /* Public */,
      11,    0,  121,    2, 0x0a /* Public */,
      12,    0,  122,    2, 0x0a /* Public */,
      13,    1,  123,    2, 0x0a /* Public */,
      14,    0,  126,    2, 0x0a /* Public */,
      15,    1,  127,    2, 0x08 /* Private */,
      16,    1,  130,    2, 0x08 /* Private */,
      18,    0,  133,    2, 0x08 /* Private */,
      19,    0,  134,    2, 0x08 /* Private */,
      20,    2,  135,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, 0x80000000 | 17,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 21,    2,    2,

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
        case 4: _t->seek((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->stop(); break;
        case 6: _t->pause(); break;
        case 7: _t->playCont(); break;
        case 8: _t->seekProp((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->seekForward(); break;
        case 10: _t->seekBackward(); break;
        case 11: _t->setDuration((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->onSeekFinished(); break;
        case 13: _t->setCurSecond((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->onAudioEnd((*reinterpret_cast< FAVPlayer*(*)>(_a[1]))); break;
        case 15: _t->onVideoEnd(); break;
        case 16: _t->check_stop(); break;
        case 17: { bool _r = _t->openPath((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const ReaderSPtr(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 14:
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
        {
            typedef void (FAVPlayer::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FAVPlayer::seek)) {
                *result = 4;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
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

// SIGNAL 4
void FAVPlayer::seek(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
