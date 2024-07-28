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
struct qt_meta_stringdata_VideoOpenGLPlayer_t {
    QByteArrayData data[29];
    char stringdata0[317];
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
QT_MOC_LITERAL(3, 28, 13), // "initProcessor"
QT_MOC_LITERAL(4, 42, 9), // "playFrame"
QT_MOC_LITERAL(5, 52, 5), // "pause"
QT_MOC_LITERAL(6, 58, 8), // "onAVStop"
QT_MOC_LITERAL(7, 67, 11), // "onPauseSeek"
QT_MOC_LITERAL(8, 79, 11), // "refreshHide"
QT_MOC_LITERAL(9, 91, 6), // "onSeek"
QT_MOC_LITERAL(10, 98, 13), // "hideBtmWidget"
QT_MOC_LITERAL(11, 112, 16), // "getPlayBtmWidget"
QT_MOC_LITERAL(12, 129, 11), // "PlayBtmBar*"
QT_MOC_LITERAL(13, 141, 12), // "initializeGL"
QT_MOC_LITERAL(14, 154, 8), // "resizeGL"
QT_MOC_LITERAL(15, 163, 8), // "window_W"
QT_MOC_LITERAL(16, 172, 8), // "window_H"
QT_MOC_LITERAL(17, 181, 7), // "paintGL"
QT_MOC_LITERAL(18, 189, 14), // "mouseMoveEvent"
QT_MOC_LITERAL(19, 204, 12), // "QMouseEvent*"
QT_MOC_LITERAL(20, 217, 5), // "event"
QT_MOC_LITERAL(21, 223, 11), // "resizeEvent"
QT_MOC_LITERAL(22, 235, 13), // "QResizeEvent*"
QT_MOC_LITERAL(23, 249, 11), // "eventFilter"
QT_MOC_LITERAL(24, 261, 7), // "watched"
QT_MOC_LITERAL(25, 269, 7), // "QEvent*"
QT_MOC_LITERAL(26, 277, 13), // "keyPressEvent"
QT_MOC_LITERAL(27, 291, 10), // "QKeyEvent*"
QT_MOC_LITERAL(28, 302, 14) // "resetBtmWidget"

    },
    "VideoOpenGLPlayer\0videoEnd\0\0initProcessor\0"
    "playFrame\0pause\0onAVStop\0onPauseSeek\0"
    "refreshHide\0onSeek\0hideBtmWidget\0"
    "getPlayBtmWidget\0PlayBtmBar*\0initializeGL\0"
    "resizeGL\0window_W\0window_H\0paintGL\0"
    "mouseMoveEvent\0QMouseEvent*\0event\0"
    "resizeEvent\0QResizeEvent*\0eventFilter\0"
    "watched\0QEvent*\0keyPressEvent\0QKeyEvent*\0"
    "resetBtmWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoOpenGLPlayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  105,    2, 0x0a /* Public */,
       4,    0,  106,    2, 0x0a /* Public */,
       5,    1,  107,    2, 0x0a /* Public */,
       6,    0,  110,    2, 0x0a /* Public */,
       7,    0,  111,    2, 0x0a /* Public */,
       8,    0,  112,    2, 0x0a /* Public */,
       9,    0,  113,    2, 0x0a /* Public */,
      10,    0,  114,    2, 0x0a /* Public */,
      11,    0,  115,    2, 0x0a /* Public */,
      13,    0,  116,    2, 0x09 /* Protected */,
      14,    2,  117,    2, 0x09 /* Protected */,
      17,    0,  122,    2, 0x09 /* Protected */,
      18,    1,  123,    2, 0x09 /* Protected */,
      21,    1,  126,    2, 0x09 /* Protected */,
      23,    2,  129,    2, 0x09 /* Protected */,
      26,    1,  134,    2, 0x09 /* Protected */,
      28,    0,  137,    2, 0x09 /* Protected */,

 // signals: parameters
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
    0x80000000 | 12,
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
        case 1: { bool _r = _t->initProcessor();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: _t->playFrame(); break;
        case 3: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onAVStop(); break;
        case 5: _t->onPauseSeek(); break;
        case 6: _t->refreshHide(); break;
        case 7: _t->onSeek(); break;
        case 8: _t->hideBtmWidget(); break;
        case 9: { PlayBtmBar* _r = _t->getPlayBtmWidget();
            if (_a[0]) *reinterpret_cast< PlayBtmBar**>(_a[0]) = std::move(_r); }  break;
        case 10: _t->initializeGL(); break;
        case 11: _t->resizeGL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->paintGL(); break;
        case 13: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 14: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 15: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 17: _t->resetBtmWidget(); break;
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
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void VideoOpenGLPlayer::videoEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
