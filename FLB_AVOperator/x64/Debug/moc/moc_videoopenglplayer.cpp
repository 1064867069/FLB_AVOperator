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
    QByteArrayData data[10];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoOpenGLPlayer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoOpenGLPlayer_t qt_meta_stringdata_VideoOpenGLPlayer = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VideoOpenGLPlayer"
QT_MOC_LITERAL(1, 18, 13), // "initProcessor"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "playFrame"
QT_MOC_LITERAL(4, 43, 5), // "pause"
QT_MOC_LITERAL(5, 49, 12), // "initializeGL"
QT_MOC_LITERAL(6, 62, 8), // "resizeGL"
QT_MOC_LITERAL(7, 71, 8), // "window_W"
QT_MOC_LITERAL(8, 80, 8), // "window_H"
QT_MOC_LITERAL(9, 89, 7) // "paintGL"

    },
    "VideoOpenGLPlayer\0initProcessor\0\0"
    "playFrame\0pause\0initializeGL\0resizeGL\0"
    "window_W\0window_H\0paintGL"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoOpenGLPlayer[] = {

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
       5,    0,   49,    2, 0x09 /* Protected */,
       6,    2,   50,    2, 0x09 /* Protected */,
       9,    0,   55,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void,

       0        // eod
};

void VideoOpenGLPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VideoOpenGLPlayer *_t = static_cast<VideoOpenGLPlayer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->initProcessor();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->playFrame(); break;
        case 2: _t->pause((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->initializeGL(); break;
        case 4: _t->resizeGL((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->paintGL(); break;
        default: ;
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
