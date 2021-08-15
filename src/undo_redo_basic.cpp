#include "undo_redo_basic.h"
#include <QDebug>

// ========================================================================================================

/*std::list<UndoRedo> undo_redo_basic::m_ur;
int undo_redo_basic::m_IDpos = 0;
int undo_redo_basic::m_IDStartPos = 0;
int undo_redo_basic::m_IDEndPos = 0;
int undo_redo_basic::m_IDmax = m_MaxMemoryID;*/

// ========================================================================================================

undo_redo_basic::undo_redo_basic()
{

}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_basic::undo(UndoRedoArgs &ura)
{
    if (!ura.SetUndoRedo())
    {
        if (availableUndo())
        {
            UndoRedo * urCurrent = nullptr;
            UndoRedo * urBefore = nullptr;
            std::list<UndoRedo>::iterator it = --m_ur.end();

            for (int i = 0; i <= m_MaxMemoryID; i++)
            {
                if (it->ID == m_IDpos)
                {
                    --m_IDpos;
                    urCurrent = &(*it);
                    --it;
                    urBefore = &(*it);
                    break;
                }
                --it;
            }

            if (urCurrent != nullptr && urBefore != nullptr)
            {
                QRect area = QRect(urCurrent->Pos.x()
                                   , urCurrent->Pos.y()
                                   , urCurrent->Size.width()
                                   , urCurrent->Size.height());
                ura.Area = ghf::AreaPixels(area.top(), area.left(), area.width(), area.height()); //urCurrent->Area;
                ura.FTexture->SetImage(urBefore->Image, area); // , true
                //ura.Image->fill(QColor(0, 0, 0, 0));
                ura.FTexture->GetImage(*ura.Image, area);
                return true;
            }
        }
    }

    qInfo() << "undo(UndoRedoArgs &ura) - end = false ----";
    return false;
}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_basic::redo(UndoRedoArgs &ura)
{
    if (!ura.SetUndoRedo())
    {
        if (availableRedo())
        {
            ++m_IDpos;
            for (UndoRedo & ur : m_ur)
            {
                if (ur.ID == m_IDpos)
                {
                    ura.Area = ur.Area;
                    QRect area = QRect(ur.Pos.x(), ur.Pos.y(), ur.Size.width(), ur.Size.height());
                    ura.FTexture->SetImage(ur.Image, area); // , true
                    ura.FTexture->GetImage(*ura.Image, area);
                    break;
                }
            }
            return true;
        }
    }
    qInfo() << "redo(UndoRedoArgs &ura) - end = false ----";
    return false;
}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_basic::set(const UndoRedoArgs &ura, bool initialize)
{
    if (ura.SetUndoRedo())
    {
        /*
         *   Zmenit to tak, aby m_IDEndPos a m_IDStartPos sa nemenili
         * */
        ghf::AreaPixels imgArea = ura.Area;
        QSize iSize = ura.Size;
        // -------------------------------------------------------------------------
        if (initialize)
        {
            iSize = ura.Image->size();
            imgArea = ghf::AreaPixels(0, 0, iSize.width(), iSize.height());
            if (m_ur.size() > 0) m_ur.clear();
            for (int i = 0; i <= m_MaxMemoryID; i++)
            {
                m_ur.push_back(UndoRedo(i, *ura.Image
                                        , imgArea));
            }
            m_IDEndPos = 0;
            m_IDStartPos = 0;
            m_IDmax = m_MaxMemoryID;
            m_IDpos = 0;
        }
        else
        {  // ---------------------------------------------------------------------
            ++m_IDpos;
            if (m_IDpos <= m_IDmax)
            {
                //QImage img(iSize, QImage::Format_RGBA8888_Premultiplied);
                //img = ura.Image->copy(imgArea.Left, imgArea.Top, imgArea.Right, imgArea.Bottom);
                for (UndoRedo & ur : m_ur)
                {
                    if (ur.ID == m_IDpos)
                    {
                        ur.setAreaPixels(imgArea);
                        ura.FTexture->GetImage(ur.Image/*, QRect(imgArea.Left, imgArea.Top, imgArea.Right, imgArea.Bottom)*/);
                        //ur = UndoRedo(m_IDpos, img, imgArea); // ura.Area
                        break;
                    }
                }
            }
            else
            {
                /*m_ur.pop_front();
                m_ur.push_back(UndoRedo(m_IDpos, *ura.Image, imgArea)); // ura.Area
                m_IDmax = m_IDpos;*/
                for (UndoRedo & ur : m_ur)
                {
                    if (ur.ID == m_IDStartPos)
                    {
                        ur.ID = m_IDpos;
                        ur.setAreaPixels(imgArea);
                        ura.FTexture->GetImage(ur.Image/*, QRect(imgArea.Left, imgArea.Top, imgArea.Right, imgArea.Bottom)*/);
                        //ur = UndoRedo(m_IDpos, img, imgArea); // ura.Area
                        break;
                    }
                }
                m_ur.sort(&UndoRedo::compare);
            }
            m_IDEndPos = m_IDpos;
            int end = m_IDEndPos - m_MaxMemoryID;
            if (end > m_IDStartPos) m_IDStartPos = end;
            if (m_ur.size() > (m_MaxMemoryID + 1))
            {
                m_ur.pop_front();
            }
        }   // ---------------------------------------------------------------------


        return true;
    }
    qInfo() << "ura.SetUndoRedo() = not set";
    return false;
}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_basic::availableRedo()
{
    if (m_IDpos < m_IDEndPos) return true;
    return false;
}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_basic::availableUndo()
{
    if (m_IDpos > m_IDStartPos) return true;
    return false;
}

// ----------------------------------------------------------------------------------------------------------------
