#ifndef UNDO_REDO_SYSTEM_H
#define UNDO_REDO_SYSTEM_H

#include <QImage>
#include <QSize>

#include <list>
#include "floattexture.h"

struct UndoRedo
{
    int ID = -1;
    QImage Image;
    QSize Size;
    QPoint Pos;

    UndoRedo(const int& id, QImage& image, QPoint position)
        : ID(id)
        , Image(image)
        , Size(image.size())
        , Pos(position)
    {}

};

// =================================================================================

struct UndoRedoArgs
{
    QImage * Image = nullptr;
    FloatTexture * FTexture = nullptr;
    QPoint Pos = QPoint(0, 0);
    QSize Size = QSize(0, 0);
private:
    bool setUndoRedo = false;
public:

    UndoRedoArgs(QImage * img, FloatTexture * ft = nullptr)
        : Image(img), FTexture(ft)
    {}

    UndoRedoArgs(const QPoint &position, const QSize &size, QImage * img, FloatTexture * ft = nullptr)
        : Image(img), FTexture(ft), Pos(position), Size(size), setUndoRedo(true)
    {}

    bool SetUndoRedo() const { return setUndoRedo; }
};

// =================================================================================

class undo_redo_system
{
    static std::list<UndoRedo> m_ur;
    static int m_IDs;
    static const int m_MaxMemoryID = 3;
    static const int m_MaxUndoRedo = 6;
    static int m_IDpos;

    undo_redo_system();
public:
    static void setup(const QSize &size);
    static bool undo(UndoRedoArgs & ura);
    static bool redo(UndoRedoArgs & ura);
    static bool set(const UndoRedoArgs & ura);

};

#endif // UNDO_REDO_SYSTEM_H
