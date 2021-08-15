#include "undo_redo_special.h"
#include <QDebug>
#include "painter_brush_debug.h"

undo_redo_special::undo_redo_special()
{

}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// UNDO

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::undo(const UndoRedoSpecialArgs &args)
{
    if (availableUndo())
    {
        --m_hPos;
        args.Image->fill(QColor(0, 0, 0, 0));
        QPainter painter(args.Image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        for (DrawHistory & dh : m_history)
        {
            if (dh.ID <= m_hPos)
            {
                if (dh.Action == historyActions::draw)
                {
                    // !!! dopisat aby nastavilo area

                    //debug_paint_brush(dh.Draw.Brush());
                    QPoint lastPoint = dh.Moves.front();
                    QPoint measurePoint = dh.Moves.front();
                    for ( QPoint & point : dh.Moves)
                    {
                        QRect area;
                        DrawManagerArgs args(&painter, &point, &area, &lastPoint, &measurePoint);
                        dh.Draw.draw(args);
                        lastPoint = point;
                    }
                }
                else if (dh.Action == historyActions::fill)
                {
                    // !!! dopisat aby nastavilo area
                    args.Image->fill(dh.Fill);
                }
                else if (dh.Action == historyActions::store_image)
                {
                    // !!! dopisat aby nastavilo area
                    *args.Image = dh.StoredImage;
                }

                qInfo() << "-- undo(args) | catch | id = " << dh.ID;
            }
            else break;
        }

        painter.end();
        return true;
    }
    qInfo() << "-- undo(args) | failed | pos = " << m_hPos;
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// REDO

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::redo(const UndoRedoSpecialArgs &args)
{
    /*
     * INFO:
        Pri redo nie je umplementovane historyActions::fill ani historyActions::store_image
        pretoze, aj tak pri nacitani obrazka alebo vycistenia platna, sa resetne undo / redo
    */
    if (availableRedo())
    {
        ++m_hPos;
        QPainter painter(args.Image);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        for (DrawHistory & dh : m_history)
        {
            if (dh.ID == m_hPos)
            {
                if (dh.Action == historyActions::draw)
                {
                    QPoint lastPoint = dh.Moves.front();
                    QPoint measurePoint = dh.Moves.front();
                    for ( QPoint & point : dh.Moves)
                    {
                        QRect area;
                        DrawManagerArgs args(&painter, &point, &area, &lastPoint, &measurePoint);
                        dh.Draw.draw(args);
                        lastPoint = point;
                    }
                }

                qInfo() << "-- redo(args) | catch | pos = " << m_hPos;
            }
            else if (dh.ID > m_hPos) break;
        }

        painter.end();
        return true;
    }
    qInfo() << "-- redo(args) | failed | pos = " << m_hPos;
   return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setINI

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::setINI(const UndoRedoSpecialSetIniArgs &args)
{
    m_imageSize = args.ImageSize;

    m_history.clear();
    m_historyCache.clear();

    m_historyActive = nullptr;
    m_activeCache = nullptr;

    m_hPos = 0;
    m_hPosMax = 0;
    m_draw_cache = 0;
    m_max_cache = 0;
    m_cachePos = 0;

    m_historyCache.push_back(QImage(m_imageSize, QImage::Format_RGBA8888_Premultiplied));
    m_activeCache = &m_historyCache.front();
    m_activeCache->fill(QColor(0, 0, 0, 0));
    return true;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setBegin

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::setBegin(const UndoRedoSpecialBeginArgs &args)
{
    qInfo() << "- setBegin()";
    if (m_historyActive == nullptr)
    {
        ++m_hPos;
        if (m_hPos > m_hPosMax)
        {
            if (args.Action == historyActions::draw)
            {
                //debug_paint_brush(args.Brush);
                m_history.push_back(DrawHistory(m_hPos, PainterBrushHolder(args.Brush)));
                m_historyActive = &m_history.back();
            }
            else if (args.Action == historyActions::fill)
            {
                m_history.push_back(DrawHistory(m_hPos, *args.Fill));
            }
            else if (args.Action == historyActions::store_image)
            {
                m_history.push_back(DrawHistory(m_hPos, *args.StoreImage));
            }
            m_hPosMax = m_hPos;
        }
        else if (m_hPos <= m_hPosMax)
        {
            for (DrawHistory & dh : m_history)
            {
                if (dh.ID == m_hPos)
                {
                    dh.Moves.clear();
                    dh.StoredImage = QImage();
                    if (args.Action == historyActions::draw)
                    {
                        //debug_paint_brush(args.Brush);
                        dh.Draw.set(args.Brush);
                        dh.Action = historyActions::draw;
                        m_historyActive = &dh;
                    }
                    else if (args.Action == historyActions::fill)
                    {
                        dh.Draw.release();
                        dh.Action = historyActions::fill;
                        dh.Fill = *args.Fill;
                    }
                    else if (args.Action == historyActions::store_image)
                    {
                        dh.Draw.release();
                        dh.Action = historyActions::store_image;
                        dh.StoredImage = *args.StoreImage;
                    }
                }
            }
        }
        qInfo() << "setBegin(args) | pos = " << m_hPos;
        return true;
    }
    m_historyActive = nullptr;
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setActive

// ----------------------------------------------------------------------------------------------------------------

void undo_redo_special::setActive(const UndoRedoSpecialActiveArgs &args)
{
    if (m_historyActive != nullptr)
    {
        m_historyActive->Moves.push_back(*args.Point);
        /*qInfo() << "setActive(args) | point x =" << args.Point->x()
                << ", y = " << args.Point->y()
                << " | moves size = " << m_historyActive->Moves.size();*/
    }
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

// setEnd

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::setEnd(const UndoRedoSpecialEndArgs &args)
{
    if (m_historyActive != nullptr)
    {
        m_historyActive->Area = args.Area;

        qInfo() << "setEnd(args) | exit true | id = " << m_historyActive->ID;
        m_historyActive = nullptr;
        return true;
    }
    return false;
}

// ================================================================================================================
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ================================================================================================================

bool undo_redo_special::availableRedo()
{
    if (m_hPos < m_hPosMax) return true;
    return false;
}

// ----------------------------------------------------------------------------------------------------------------

bool undo_redo_special::availableUndo()
{
    if (m_hPos > m_hPosStart) return true;
    return false;
}

// ----------------------------------------------------------------------------------------------------------------
