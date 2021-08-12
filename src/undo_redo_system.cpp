#include "undo_redo_system.h"

// ========================================================================================================
std::list<UndoRedo> undo_redo_system::m_ur;
int undo_redo_system::m_IDs = 0;
int undo_redo_system::m_IDpos = 0;

// ========================================================================================================

undo_redo_system::undo_redo_system()
{

}

void undo_redo_system::setup(const QSize &size)
{
    QImage img(size, QImage::Format_RGBA8888_Premultiplied);
    img.fill(QColor(0, 0, 0, 0));
    if (m_ur.size() == 0)
    {
        for (int i = 0; i < m_MaxMemoryID; i++)
        {
            m_ur.push_back(UndoRedo(i, img, QPoint(0, 0)));
        }
    }
    else
    {
        std::list<UndoRedo>::iterator it = m_ur.begin();
        for (int i = 0; i < m_ur.size(); i++)
        {
            *it = UndoRedo(i, img, QPoint(0, 0));
        }
    }
}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::undo(UndoRedoArgs &ura)
{
    return false;
}

// --------------------------------------------------------------------------------------------------------

bool undo_redo_system::redo(UndoRedoArgs &ura)
{
    return false;
}

bool undo_redo_system::set(const UndoRedoArgs &ura)
{
    if (ura.SetUndoRedo())
    {
        if (m_ur.size() < m_MaxMemoryID)
        {
            QImage img(ura.Size, QImage::Format_RGBA8888_Premultiplied);
            img = ura.Image->copy(ura.Pos.x(), ura.Pos.y(), ura.Size .width(), ura.Size.height());
            m_ur.push_back(UndoRedo(m_IDs, img, ura.Pos));
        }
    }
    return false;
}

// --------------------------------------------------------------------------------------------------------
