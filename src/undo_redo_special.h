#ifndef UNDO_REDO_SPECIAL_H
#define UNDO_REDO_SPECIAL_H

#include "painter_brush.h"
#include <list>
#include <QPoint>
#include <QImage>
#include <QPainter>

#include "undo_redo_file.h"

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
    bool DrawCached = false;
    bool Cached = false;

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

struct CanvasHistory
{
    int ID = 0;
    QImage Canvas;
    bool Cached = false;

    CanvasHistory() {}
    CanvasHistory(const int& id, const QImage & image)
        : ID(id), Canvas(image)
    {}
};

// ========================================================================================================

struct UndoRedoSpecialSetIniArgs
{
    QSize ImageSize;
    QImage * Image = nullptr;
    UndoRedoSpecialSetIniArgs(const QSize & imgSize)
        : ImageSize(imgSize)
    {}

    UndoRedoSpecialSetIniArgs(const QSize & imgSize, QImage * image)
        : ImageSize(imgSize)
        , Image(image)
    {}

};

// ========================================================================================================

struct UndoRedoSpecialArgs
{
    ghf::AreaPixels & Area;
    QImage * Image = nullptr;

    UndoRedoSpecialArgs(QImage * image, ghf::AreaPixels & area)
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
    QImage * Image = nullptr;
    FloatTexture * FTexture = nullptr;

    UndoRedoSpecialEndArgs(const ghf::AreaPixels & area)
        : Area(area)
    {}

    UndoRedoSpecialEndArgs(const ghf::AreaPixels & area
                           , QImage * image)
        : Area(area)
        , Image(image)
    {}

    UndoRedoSpecialEndArgs(const ghf::AreaPixels & area
                           , QImage * image
                           , FloatTexture * floatedTexture)
        : Area(area)
        , Image(image)
        , FTexture(floatedTexture)
    {}
};

// ========================================================================================================

class undo_redo_special
{
    QSize m_imageSize;
    std::list<DrawHistory> m_history;   // historia tahov
    std::list<CanvasHistory> m_historyCanvasCache;  // historia platna (zachytava kazdy desiaty tah dozadu) MAX_CANVAS_CACHE
    int m_hPos = 0;                     // pozicia pre undo / redo tahy
    int m_hPosMax = 0;                  // kolko je undo / redo tahov
    const int m_hPosStart = 1;          // zaciatocna pozicia pre undo / redo
    // ulozenie tahov do platna
    const int MAX_CANVAS_CACHE = 10;    // urcuje maximalny rozsah pre m_historyCanvasCache
    int m_canvas_cache = 0;             // nieco ako m_hPos len pre m_historyCanvasCache
    int m_canvas_cache_MAX = 0;         // maximalna hodnota na posunutie pozicie pre m_historyCanvasCache
    int m_canvas_cache_pos = 0;         // pozicia pre m_historyCanvasCache
    // ulozenie do noveho platna
    const int MAX_CACHE = 10;           // maximalny cache pre historiu tahov
    int m_max_cache = 0;                //
    int m_cachePos = 0;                 //

    bool m_drawIntoCache = false;       //
    int m_draw_moves = 0;
    const int m_draw_moves_MAX = 20;

    DrawHistory * m_historyActive = nullptr;        // aktualna historia tahov
    CanvasHistory * m_activeCanvasCache = nullptr;  // aktualne historia platna
    bool m_useCanvasCache = false;                  // ci pouzit historiu platna pri undo alebo nie

    undo_redo_file m_fileCache;         // uklada a vybera cache obrazky z disku pre m_history a m_historyCanvasCache
public:
    undo_redo_special();

    void setFileCachePath(std::string path);

    bool oldUndo(UndoRedoSpecialArgs& args);
    bool undo(UndoRedoSpecialArgs& args);
    bool redo(UndoRedoSpecialArgs& args);
private:
    bool undoRedoCache(UndoRedoSpecialArgs& args, bool Forward);
    void setCache(const UndoRedoSpecialEndArgs &args);
public:

    bool setINI(const UndoRedoSpecialSetIniArgs &args);
    bool setBegin(const UndoRedoSpecialBeginArgs &args);
    void setActive(const UndoRedoSpecialActiveArgs &args);
    bool setEnd(const UndoRedoSpecialEndArgs &args);

    bool availableRedo();
    bool availableUndo();

};

// ========================================================================================================

#endif // UNDO_REDO_SPECIAL_H
