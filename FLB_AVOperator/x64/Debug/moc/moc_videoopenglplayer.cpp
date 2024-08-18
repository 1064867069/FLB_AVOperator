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
    QByteArrayData data[8];
    char stringdata0[76];
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
QT_MOC_LITERAL(3, 35, 4), // "stop"
QT_MOC_LITERAL(4, 40, 5), // "clear"
QT_MOC_LITERAL(5, 46, 3), // "lck"
QT_MOC_LITERAL(6, 50, 15), // "updateLastFrame"
QT_MOC_LITERAL(7, 66, 9) // "FrameSPtr"

    },
    "VideoFrameReadManager\0procVFrames\0\0"
    "stop\0clear\0lck\0updateLastFrame\0FrameSPtr"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoFrameReadManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    1,   46,    2, 0x0a /* Public */,
       4,    0,   49,    2, 0x2a /* Public | MethodCloned */,
       6,    1,   50,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x2a /* Public | MethodCloned */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    0x80000000 | 7, 0x80000000 | 7,    2,
    0x80000000 | 7,

       0        // eod
};

void VideoFrameReadManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoFrameReadManager *_t = static_cast<VideoFrameReadManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->procVFrames(); break;
        case 1: _t->stop(); break;
        case 2: _t->clear((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->clear(); break;
        case 4: { FrameSPtr _r = _t->updateLastFrame((*reinterpret_cast< const FrameSPtr(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< FrameSPtr*>(_a[0]) = std::move(_r); }  break;
        case 5: { FrameSPtr _r = _t->updateLastFrame();
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
struct qt_meta_stringdata_VideoOpenGLPlayer_t {
    QByteArrayData data[29];
    char stringdata0[313];
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
QT_MOC_LITERAL(4, 37, 13), // "initProcessor"
QT_MOC_LITERAL(5, 51, 9), // "playFrame"
QT_MOC_LITERAL(6, 61, 5), // "pause"
QT_MOC_LITERAL(7, 67, 8), // "onAVStop"
QT_MOC_LITERAL(8, 76, 11), // "onPauseSeek"
QT_MOC_LITERAL(9, 88, 11), // "refreshHide"
QT_MOC_LITERAL(10, 100, 6), // "onSeek"
QT_MOC_LITERAL(11, 107, 13), // "hideBtmWidget"
QT_MOC_LITERAL(12, 121, 15), // "onParamsUpdated"
QT_MOC_LITERAL(13, 137, 12), // "initializeGL"
QT_MOC_LITERAL(14, 150, 8), // "resizeGL"
QT_MOC_LITERAL(15, 159, 8), // "window_W"
QT_MOC_LITERAL(16, 168, 8), // "window_H"
QT_MOC_LITERAL(17, 177, 7), // "paintGL"
QT_MOC_LITERAL(18, 185, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(19, 200, 12), // "QMouseEvent*"
QT_MOC_LITERAL(20, 213, 5), // "event"
QT_MOC_LITERAL(21, 219, 11), // "resizeEvent"
QT_MOC_LITERAL(22, 231, 13), // "QResizeEvent*"
QT_MOC_LITERAL(23, 245, 11), // "eventFilter"
QT_MOC_LITERAL(24, 257, 7), // "watched"
QT_MOC_LITERAL(25, 265, 7), // "QEvent*"
QT_MOC_LITERAL(26, 273, 13), // "keyPressEvent"
QT_MOC_LITERAL(27, 287, 10), // "QKeyEvent*"
QT_MOC_LITERAL(28, 298, 14) // "resetBtmWidget"

    },
    "VideoOpenGLPlayer\0videoEnd\0\0needProc\0"
    "initProcessor\0playFrame\0pause\0onAVStop\0"
    "onPauseSeek\0refreshHide\0onSeek\0"
    "hideBtmWidget\0onParamsUpdated\0"
    "initializeGL\0resizeGL\0window_W\0window_H\0"
    "paintGL\0mouseMoveEvent\0QMouseEvent*\0"
    "event\0resizeEvent\0QResizeEvent*\0"
    "eventFilter\0watched\0QEvent*\0keyPressEvent\0"
    "QKeyEvent*\0resetBtmWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoOpenGLPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  109,    2, 0x06 /* Public */,
       3,    0,  110,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,  111,    2, 0x0a /* Public */,
       5,    0,  112,    2, 0x0a /* Public */,
       6,    1,  113,    2, 0x0a /* Public */,
       7,    0,  116,    2, 0x0a /* Public */,
       8,    0,  117,    2, 0x0a /* Public */,
       9,    0,  118,    2, 0x0a /* Public */,
      10,    0,  119,    2, 0x0a /* Public */,
      11,    0,  120,    2, 0x0a /* Public */,
      12,    0,  121,    2, 0x0a /* Public */,
      13,    0,  122,    2, 0x09 /* Protected */,
      14,    2,  123,    2, 0x09 /* Protected */,
      17,    0,  128,    2, 0x09 /* Protected */,
      18,    1,  129,    2, 0x09 /* Protected */,
      21,    1,  132,    2, 0x09 /* Protected */,
      23,    2,  135,    2, 0x09 /* Protected */,
      26,    1,  140,    2, 0x09 /* Protected */,
      28,    0,  143,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   15,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 22,   20,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 25,   24,   20,
    QMetaType::Void, 0x80000000 | 27,   20,
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
        case 2: { bool _r = _t->initProcessor();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->playFrame(); break;
        case 4: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->onAVStop(); break;
        case 6: _t->onPauseSeek(); break;
        case 7: _t->refreshHide(); break;
        case 8: _t->onSeek(); break;
        case 9: _t->hideBtmWidget(); break;
        case 10: _t->onParamsUpdated(); break;
        case 11: _t->initializeGL(); break;
        case 12: _t->resizeGL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->paintGL(); break;
        case 14: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 15: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 16: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 18: _t->resetBtmWidget(); break;
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
