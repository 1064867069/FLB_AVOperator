/****************************************************************************
** Meta object code from reading C++ file 'playtable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../playtable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playtable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlayTableBase_t {
    QByteArrayData data[9];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlayTableBase_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlayTableBase_t qt_meta_stringdata_PlayTableBase = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PlayTableBase"
QT_MOC_LITERAL(1, 14, 6), // "played"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 7), // "lastAVF"
QT_MOC_LITERAL(4, 30, 7), // "nextAVF"
QT_MOC_LITERAL(5, 38, 11), // "play_cancel"
QT_MOC_LITERAL(6, 50, 11), // "resizeEvent"
QT_MOC_LITERAL(7, 62, 13), // "QResizeEvent*"
QT_MOC_LITERAL(8, 76, 5) // "event"

    },
    "PlayTableBase\0played\0\0lastAVF\0nextAVF\0"
    "play_cancel\0resizeEvent\0QResizeEvent*\0"
    "event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayTableBase[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    1,   43,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void PlayTableBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlayTableBase *_t = static_cast<PlayTableBase *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->played(); break;
        case 1: _t->lastAVF(); break;
        case 2: _t->nextAVF(); break;
        case 3: _t->play_cancel(); break;
        case 4: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (PlayTableBase::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PlayTableBase::played)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject PlayTableBase::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_PlayTableBase.data,
      qt_meta_data_PlayTableBase,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PlayTableBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlayTableBase::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlayTableBase.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int PlayTableBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void PlayTableBase::played()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_PathPlayTable_t {
    QByteArrayData data[14];
    char stringdata0[157];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PathPlayTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PathPlayTable_t qt_meta_stringdata_PathPlayTable = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PathPlayTable"
QT_MOC_LITERAL(1, 14, 7), // "playAVF"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "addNewFile"
QT_MOC_LITERAL(4, 34, 7), // "lastAVF"
QT_MOC_LITERAL(5, 42, 7), // "nextAVF"
QT_MOC_LITERAL(6, 50, 21), // "mouseDoubleClickEvent"
QT_MOC_LITERAL(7, 72, 12), // "QMouseEvent*"
QT_MOC_LITERAL(8, 85, 5), // "event"
QT_MOC_LITERAL(9, 91, 17), // "mouseReleaseEvent"
QT_MOC_LITERAL(10, 109, 7), // "delFile"
QT_MOC_LITERAL(11, 117, 19), // "onCustomContextMenu"
QT_MOC_LITERAL(12, 137, 3), // "pos"
QT_MOC_LITERAL(13, 141, 15) // "onPlayTriggered"

    },
    "PathPlayTable\0playAVF\0\0addNewFile\0"
    "lastAVF\0nextAVF\0mouseDoubleClickEvent\0"
    "QMouseEvent*\0event\0mouseReleaseEvent\0"
    "delFile\0onCustomContextMenu\0pos\0"
    "onPlayTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PathPlayTable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   62,    2, 0x0a /* Public */,
       4,    0,   65,    2, 0x0a /* Public */,
       5,    0,   66,    2, 0x0a /* Public */,
       6,    1,   67,    2, 0x09 /* Protected */,
       9,    1,   70,    2, 0x09 /* Protected */,
      10,    0,   73,    2, 0x09 /* Protected */,
      11,    1,   74,    2, 0x09 /* Protected */,
      13,    0,   77,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void,

       0        // eod
};

void PathPlayTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PathPlayTable *_t = static_cast<PathPlayTable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->playAVF((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->addNewFile((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->lastAVF(); break;
        case 3: _t->nextAVF(); break;
        case 4: _t->mouseDoubleClickEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: _t->mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 6: _t->delFile(); break;
        case 7: _t->onCustomContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 8: _t->onPlayTriggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (PathPlayTable::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PathPlayTable::playAVF)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject PathPlayTable::staticMetaObject = {
    { &PlayTableBase::staticMetaObject, qt_meta_stringdata_PathPlayTable.data,
      qt_meta_data_PathPlayTable,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PathPlayTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PathPlayTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PathPlayTable.stringdata0))
        return static_cast<void*>(this);
    return PlayTableBase::qt_metacast(_clname);
}

int PathPlayTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PlayTableBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PathPlayTable::playAVF(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
