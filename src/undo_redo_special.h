#ifndef UNDO_REDO_SPECIAL_H
#define UNDO_REDO_SPECIAL_H

#include "painter_brush.h"
#include <list>
#include <QPoint>
#include <QImage>
#include <QPainter>

// ========================================================================================================
enum class historyActions
{
    draw
    , fill
    , store_image
};

// ========================================================================================================

struct DrawHistory
{
    int ID = 0;
    PainterBrushHolder Draw;
    std::list<QPoint> Moves;
    ghf::AreaPixels Area;
    historyActions Action = historyActions::draw;
    QImage StoredImage;
    QColor Fill;

    DrawHistory(const int & id
                , const PainterBrushHolder & pbh
                )
        : ID(id)
        , Draw(pbh)
    {}

    DrawHistory(const int & id
                , const PainterBrushHolder & pbh
                , const std::list<QPoint> & moves
                , const ghf::AreaPixels & area
                )
        : ID(id)
        , Draw(pbh)
        , Moves(moves)
        , Area(area)
    {}

    DrawHistory(const int & id
                , const QImage & image
                )
        : ID(id)
        , Draw(nullptr)
        , Action(historyActions::store_image)
        , StoredImage(image)
    { }

    DrawHistory(const int & id
                , const QColor & fill
                )
        : ID(id)
        , Draw(nullptr)
        , Action(historyActions::fill)
        , Fill(fill)
    { }
};

// ========================================================================================================

struct UndoRedoSpecialSetIniArgs
{
    QSize ImageSize;
    UndoRedoSpecialSetIniArgs(const QSize & imgSize)
        : ImageSize(imgSize)
    {}

};

// ========================================================================================================

struct UndoRedoSpecialArgs
{
    ghf::AreaPixels Area;
    QImage * Image = nullptr;

    UndoRedoSpecialArgs(QImage * image, const ghf::AreaPixels & area)
        : Area(area)
        , Image(image)
    {}
};

// ========================================================================================================

struct UndoRedoSpecialBeginArgs
{
    painter_brush * Brush = nullptr;
    const QPoint * StartPoint = nullptr;
    historyActions Action = historyActions::draw;
    QImage * StoreImage = nullptr;
    QColor * Fill = nullptr;

    UndoRedoSpecialBeginArgs(painter_brush * brush, const QPoint * startPoint)
        : Brush(brush)
        , StartPoint(startPoint)
    {}

    UndoRedoSpecialBeginArgs(QImage * image)
        : Brush(nullptr)
        , Action(historyActions::store_image)
        , StoreImage(image)
    {}

    UndoRedoSpecialBeginArgs(QColor * fill)
        : Brush(nullptr)
        , Action(historyActions::fill)
        , Fill(fill)
    {}
};

struct UndoRedoSpecialActiveArgs
{
    const QPoint * Point = nullptr;
    UndoRedoSpecialActiveArgs(const QPoint * point)
        : Point(point)
    {}
};

struct UndoRedoSpecialEndArgs
{
    ghf::AreaPixels Area;

    UndoRedoSpecialEndArgs(const ghf::AreaPixels & area)
        : Area(area)
    {}
};

// ========================================================================================================

class undo_redo_special
{
    QSize m_imageSize;
    std::list<DrawHistory> m_history;
    std::list<QImage> m_historyCache;
    int m_hPos = 0;
    int m_hPosMax = 0;
    const int m_hPosStart = 0;
    // ulozenie tahov do platna
    const int MAX_DRAW_CACHE = 10;
    int m_draw_cache = 0;
    // ulozenie do noveho platna
    const int MAX_CACHE = 50;
    int m_max_cache = 0;
    int m_cachePos = 0;

    DrawHistory * m_historyActive = nullptr;
    QImage * m_activeCache = nullptr;
public:
    undo_redo_special();

    bool undo(const UndoRedoSpecialArgs& args);
    bool redo(const UndoRedoSpecialArgs& args);

    bool setINI(const UndoRedoSpecialSetIniArgs &args);
    bool setBegin(const UndoRedoSpecialBeginArgs &args);
    void setActive(const UndoRedoSpecialActiveArgs &args);
    bool setEnd(const UndoRedoSpecialEndArgs &args);

    bool availableRedo();
    bool availableUndo();

};

// ========================================================================================================

#endif // UNDO_REDO_SPECIAL_H
