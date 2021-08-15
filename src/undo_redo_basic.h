#ifndef UNDO_REDO_BASIC_H
#define UNDO_REDO_BASIC_H

#include <QImage>
#include <QSize>

#include <list>
#include "floattexture.h"
#include "graphic_helper_functions.h"


struct UndoRedo
{
    int ID = -1;
    QImage Image;
    QSize Size = QSize(0, 0);
    QPoint Pos = QPoint(0, 0);
    ghf::AreaPixels Area = { 0, 0, 0, 0 };

    void setAreaPixels(const ghf::AreaPixels & area)
    {
        Area = area;
        Size = QSize(Area.Right, Area.Bottom);
        Pos = QPoint(Area.Left, Area.Top);
    }

    /*UndoRedo(const int& id, QImage& image, const QPoint &position)
        : ID(id)
        , Image(image)
        , Size(image.size())
        , Pos(position)
    {}*/

    UndoRedo(const int& id, QImage& image, const ghf::AreaPixels & area)
        : ID(id)
        , Image(image)
    { setAreaPixels(area); }

    static bool compare(const UndoRedo& left, const UndoRedo& right)
    {
        if (left.ID < right.ID) return true;
        else return false;
    }

};

// =================================================================================

struct UndoRedoArgs
{
    QImage * Image = nullptr;
    FloatTexture * FTexture = nullptr;
    QPoint Pos = QPoint(0, 0);
    QSize Size = QSize(0, 0);
    ghf::AreaPixels Area = { 0, 0, 0, 0 };
private:
    bool setUndoRedo = false;

    void setAreaPixels(const ghf::AreaPixels & area)
    {
        Area = area;
        Size = QSize(Area.Right, Area.Bottom);
        Pos = QPoint(Area.Left, Area.Top);
    }
public:


    UndoRedoArgs(QImage * img, FloatTexture * ft = nullptr)
        : Image(img), FTexture(ft)
    {}

    UndoRedoArgs(const QPoint &position, const QSize &size, QImage * img, FloatTexture * ft = nullptr)
        : Image(img), FTexture(ft), setUndoRedo(true)
    { setAreaPixels(ghf::AreaPixels(position.y(), position.x(), size.width(), size.height())); }

    UndoRedoArgs( const ghf::AreaPixels & area, QImage * img, FloatTexture * ft = nullptr)
        : Image(img), FTexture(ft), setUndoRedo(true)
    { setAreaPixels(area); }

    bool SetUndoRedo() const { return setUndoRedo; }
};

// =================================================================================

class undo_redo_basic
{
    std::list<UndoRedo> m_ur;
    // maximalna pamat pre undo redo
    const int m_MaxMemoryID = 3;
    // maximalny krok spat pre undo redo
    const int m_MaxUndoRedo = 6;
    // pozicia pre undo redo
    int m_IDpos = 0;
    // zaciatocna pozicia pre undo redo (ak, undo redo dosiahlo maximum, m_IDStartPos
    // si zapamata odkial ma pracovat ako prvy bod.
    // pociatocna hodnota je 0
    int m_IDStartPos = 0;
    int m_IDEndPos = 0;
    int m_IDmax = 0;
public:
    undo_redo_basic();

    bool undo(UndoRedoArgs & ura);
    bool redo(UndoRedoArgs & ura);
    bool set(const UndoRedoArgs & ura, bool initialize = false);
    bool availableRedo();
    bool availableUndo();
};

#endif // UNDO_REDO_BASIC_H
