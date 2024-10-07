/****************************************************************************
** Meta object code from reading C++ file 'videoopenglplayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../videoopenglplayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videoopenglplayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoFrameReadManager_t {
    QByteArrayData data[11];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoFrameReadManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoFrameReadManager_t qt_meta_stringdata_VideoFrameReadManager = {
    {
QT_MOC_LITERAL(0, 0, 21), // "VideoFrameReadManager"
QT_MOC_LITERAL(1, 22, 11), // "procVFrames"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 11), // "recoverProc"
QT_MOC_LITERAL(4, 47, 4), // "stop"
QT_MOC_LITERAL(5, 52, 5), // "clear"
QT_MOC_LITERAL(6, 58, 3), // "lck"
QT_MOC_LITERAL(7, 62, 4), // "seek"
QT_MOC_LITERAL(8, 67, 3), // "sec"
QT_MOC_LITERAL(9, 71, 15), // "updateLastFrame"
QT_MOC_LITERAL(10, 87, 9) // "FrameSPtr"

    },
    "VideoFrameReadManager\0procVFrames\0\0"
    "recoverProc\0stop\0clear\0lck\0seek\0sec\0"
    "updateLastFrame\0FrameSPtr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoFrameReadManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    1,   57,    2, 0x0a /* Public */,
       5,    0,   60,    2, 0x2a /* Public | MethodCloned */,
       7,    1,   61,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
       9,    0,   67,    2, 0x2a /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    8,
    0x80000000 | 10, 0x80000000 | 10,    2,
    0x80000000 | 10,

       0        // eod
};

void VideoFrameReadManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoFrameReadManager *_t = static_cast<VideoFrameReadManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->procVFrames(); break;
        case 1: _t->recoverProc(); break;
        case 2: _t->stop(); break;
        case 3: _t->clear((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->clear(); break;
        case 5: _t->seek((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: { FrameSPtr _r = _t->updateLastFrame((*reinterpret_cast< const FrameSPtr(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< FrameSPtr*>(_a[0]) = std::move(_r); }  break;
        case 7: { FrameSPtr _r = _t->updateLastFrame();
            if (_a[0]) *reinterpret_cast< FrameSPtr*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject VideoFrameReadManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VideoFrameReadManager.data,
      qt_meta_data_VideoFrameReadManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoFrameReadManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoFrameReadManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoFrameReadManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoFrameReadManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
struct qt_meta_stringdata_VideoOpenGLPlayer_t {
    QByteArrayData data[36];
    char stringdata0[411];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoOpenGLPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoOpenGLPlayer_t qt_meta_stringdata_VideoOpenGLPlayer = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VideoOpenGLPlayer"
QT_MOC_LITERAL(1, 18, 8), // "videoEnd"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "needProc"
QT_MOC_LITERAL(4, 37, 11), // "willDestroy"
QT_MOC_LITERAL(5, 49, 11), // "rectChanged"
QT_MOC_LITERAL(6, 61, 19), // "curPlayFrameChanged"
QT_MOC_LITERAL(7, 81, 9), // "FrameSPtr"
QT_MOC_LITERAL(8, 91, 13), // "initProcessor"
QT_MOC_LITERAL(9, 105, 9), // "playFrame"
QT_MOC_LITERAL(10, 115, 12), // "repaintFrame"
QT_MOC_LITERAL(11, 128, 5), // "pause"
QT_MOC_LITERAL(12, 134, 8), // "onAVStop"
QT_MOC_LITERAL(13, 143, 11), // "onPauseSeek"
QT_MOC_LITERAL(14, 155, 14), // "onSeekFinished"
QT_MOC_LITERAL(15, 170, 11), // "refreshHide"
QT_MOC_LITERAL(16, 182, 6), // "onSeek"
QT_MOC_LITERAL(17, 189, 13), // "hideBtmWidget"
QT_MOC_LITERAL(18, 203, 15), // "onParamsUpdated"
QT_MOC_LITERAL(19, 219, 12), // "initializeGL"
QT_MOC_LITERAL(20, 232, 8), // "resizeGL"
QT_MOC_LITERAL(21, 241, 8), // "window_W"
QT_MOC_LITERAL(22, 250, 8), // "window_H"
QT_MOC_LITERAL(23, 259, 7), // "paintGL"
QT_MOC_LITERAL(24, 267, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(25, 282, 12), // "QMouseEvent*"
QT_MOC_LITERAL(26, 295, 5), // "event"
QT_MOC_LITERAL(27, 301, 15), // "mousePressEvent"
QT_MOC_LITERAL(28, 317, 11), // "resizeEvent"
QT_MOC_LITERAL(29, 329, 13), // "QResizeEvent*"
QT_MOC_LITERAL(30, 343, 11), // "eventFilter"
QT_MOC_LITERAL(31, 355, 7), // "watched"
QT_MOC_LITERAL(32, 363, 7), // "QEvent*"
QT_MOC_LITERAL(33, 371, 13), // "keyPressEvent"
QT_MOC_LITERAL(34, 385, 10), // "QKeyEvent*"
QT_MOC_LITERAL(35, 396, 14) // "resetBtmWidget"

    },
    "VideoOpenGLPlayer\0videoEnd\0\0needProc\0"
    "willDestroy\0rectChanged\0curPlayFrameChanged\0"
    "FrameSPtr\0initProcessor\0playFrame\0"
    "repaintFrame\0pause\0onAVStop\0onPauseSeek\0"
    "onSeekFinished\0refreshHide\0onSeek\0"
    "hideBtmWidget\0onParamsUpdated\0"
    "initializeGL\0resizeGL\0window_W\0window_H\0"
    "paintGL\0mouseMoveEvent\0QMouseEvent*\0"
    "event\0mousePressEvent\0resizeEvent\0"
    "QResizeEvent*\0eventFilter\0watched\0"
    "QEvent*\0keyPressEvent\0QKeyEvent*\0"
    "resetBtmWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoOpenGLPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  139,    2, 0x06 /* Public */,
       3,    0,  140,    2, 0x06 /* Public */,
       4,    0,  141,    2, 0x06 /* Public */,
       5,    1,  142,    2, 0x06 /* Public */,
       6,    1,  145,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  148,    2, 0x0a /* Public */,
       9,    0,  149,    2, 0x0a /* Public */,
      10,    0,  150,    2, 0x0a /* Public */,
      11,    1,  151,    2, 0x0a /* Public */,
      12,    0,  154,    2, 0x0a /* Public */,
      13,    0,  155,    2, 0x0a /* Public */,
      14,    0,  156,    2, 0x0a /* Public */,
      15,    0,  157,    2, 0x0a /* Public */,
      16,    0,  158,    2, 0x0a /* Public */,
      17,    0,  159,    2, 0x0a /* Public */,
      18,    0,  160,    2, 0x0a /* Public */,
      19,    0,  161,    2, 0x09 /* Protected */,
      20,    2,  162,    2, 0x09 /* Protected */,
      23,    0,  167,    2, 0x09 /* Protected */,
      24,    1,  168,    2, 0x09 /* Protected */,
      27,    1,  171,    2, 0x09 /* Protected */,
      28,    1,  174,    2, 0x09 /* Protected */,
      30,    2,  177,    2, 0x09 /* Protected */,
      33,    1,  182,    2, 0x09 /* Protected */,
      35,    0,  185,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect,    2,
    QMetaType::Void, 0x80000000 | 7,    2,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   21,   22,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 29,   26,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 32,   31,   26,
    QMetaType::Void, 0x80000000 | 34,   26,
    QMetaType::Void,

       0        // eod
};

void VideoOpenGLPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoOpenGLPlayer *_t = static_cast<VideoOpenGLPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->videoEnd(); break;
        case 1: _t->needProc(); break;
        case 2: _t->willDestroy(); break;
        case 3: _t->rectChanged((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 4: _t->curPlayFrameChanged((*reinterpret_cast< FrameSPtr(*)>(_a[1]))); break;
        case 5: { bool _r = _t->initProcessor();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->playFrame(); break;
        case 7: _t->repaintFrame(); break;
        case 8: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->onAVStop(); break;
        case 10: _t->onPauseSeek(); break;
        case 11: _t->onSeekFinished(); break;
        case 12: _t->refreshHide(); break;
        case 13: _t->onSeek(); break;
        case 14: _t->hideBtmWidget(); break;
        case 15: _t->onParamsUpdated(); break;
        case 16: _t->initializeGL(); break;
        case 17: _t->resizeGL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->paintGL(); break;
        case 19: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 20: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 21: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 22: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 23: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 24: _t->resetBtmWidget(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VideoOpenGLPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOpenGLPlayer::videoEnd)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VideoOpenGLPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOpenGLPlayer::needProc)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VideoOpenGLPlayer::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOpenGLPlayer::willDestroy)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (VideoOpenGLPlayer::*_t)(const QRect & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOpenGLPlayer::rectChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (VideoOpenGLPlayer::*_t)(FrameSPtr );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoOpenGLPlayer::curPlayFrameChanged)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject VideoOpenGLPlayer::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_VideoOpenGLPlayer.data,
      qt_meta_data_VideoOpenGLPlayer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VideoOpenGLPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoOpenGLPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoOpenGLPlayer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int VideoOpenGLPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void VideoOpenGLPlayer::videoEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void VideoOpenGLPlayer::needProc()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VideoOpenGLPlayer::willDestroy()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void VideoOpenGLPlayer::rectChanged(const QRect & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VideoOpenGLPlayer::curPlayFrameChanged(FrameSPtr _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
