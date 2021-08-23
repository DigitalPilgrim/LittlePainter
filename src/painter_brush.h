#ifndef PAINTER_BRUSH_H
#define PAINTER_BRUSH_H

#include <QImage>
#include <QPainter>
#include <QPoint>
#include "image_manager.h"
#include "graphic_helper_functions.h"
#include "floattexture.h"
#include <vector>

enum class e_brushType
{
    not_set = -1
    , pen = 0
    , circleBrush
    , textureBrush
};

// =============================================================================

struct DrawManagerArgs
{
    QPainter * Painter = nullptr;
    FloatTexture * FTexture = nullptr;
    const QPoint * EndPoint = nullptr;
    QRect * Area = nullptr;
    QImage * DrawArea = nullptr; // zatial zbytocne

    QPoint * LastPoint = nullptr;
    QPoint * MeasurePoint = nullptr;

    ghf::AreaPixels * PixelArea = nullptr;

    DrawManagerArgs(QPainter * painter, const QPoint * endPoint, QRect * area, QImage * drawArea = nullptr)
        : Painter(painter), EndPoint(endPoint), Area(area), DrawArea(drawArea)
    {}

    DrawManagerArgs(QPainter * painter
                    , const QPoint * endPoint
                    , QRect * area
                    , QPoint * lastPoint
                    , QPoint * measurePoint
                    , QImage * drawArea = nullptr)
        : Painter(painter)
        , EndPoint(endPoint)
        , Area(area)
        , DrawArea(drawArea)
        , LastPoint(lastPoint)
        , MeasurePoint(measurePoint)
    {}

    DrawManagerArgs(QPainter * painter
                    , const QPoint * endPoint
                    , QRect * area
                    , QPoint * lastPoint
                    , QPoint * measurePoint
                    , ghf::AreaPixels * pixelArea
                    , QImage * drawArea = nullptr)
        : Painter(painter)
        , EndPoint(endPoint)
        , Area(area)
        , DrawArea(drawArea)
        , LastPoint(lastPoint)
        , MeasurePoint(measurePoint)
        , PixelArea(pixelArea)
    {}

    DrawManagerArgs(QPainter * painter
                    , FloatTexture * fTexture
                    , const QPoint * endPoint
                    , QRect * area
                    , QPoint * lastPoint
                    , QPoint * measurePoint
                    , QImage * drawArea = nullptr)
        : Painter(painter)
        , FTexture(fTexture)
        , EndPoint(endPoint)
        , Area(area)
        , DrawArea(drawArea)
        , LastPoint(lastPoint)
        , MeasurePoint(measurePoint)
    {}
};

// =============================================================================

struct painter_brush
{
    QImage OriginalImageTexture;
    std::vector<std::vector<int>> OriginalImageAlpha;
    QImage Image;
    QColor Color = QColor(128, 128, 128, 255);
    int Width = 1;
    float Alpha = 1.0f;
    e_brushType Type = e_brushType::not_set;
    e_imageBrush ImageBrushSelected = e_imageBrush::texture1;

    painter_brush() {}
    painter_brush(e_brushType value) : Type(value) {}
    virtual ~painter_brush() {}

    void set(const painter_brush * pb);

    virtual void draw(const DrawManagerArgs& args) {}
    virtual void setImageColor(const QColor &value) {}
    virtual void setImageAlpha(const float &alpha, bool recreate = false) {}
    virtual void resize() {}
    virtual void reinitialize() {}
};

// =============================================================================

class PainterBrushHolder
{
    painter_brush * pb = nullptr;
public:
    PainterBrushHolder(const PainterBrushHolder& pbh);
    PainterBrushHolder(painter_brush * painterBrush);
    ~PainterBrushHolder();
    void set(painter_brush * painterBrush);
private:
    bool create(painter_brush * painterBrush);
public:
    bool isLive() const { if (pb != nullptr) return true; else return false; }
    void draw(const DrawManagerArgs& args);
    void setImageColor(const QColor & value);
    painter_brush * Brush() { return pb; };
    void release();
};

// =============================================================================


#endif // PAINTER_BRUSH_H
