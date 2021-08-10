#ifndef PAINTER_BRUSH_H
#define PAINTER_BRUSH_H

#include <QImage>
#include <QPainter>
#include "image_manager.h"

enum class e_brushType
{
    not_set = -1
    , pen = 0
    , circleBrush
    , textureBrush
};

// =============================================================================

struct painter_brush
{
    QImage Image;
    QColor Color = QColor(128, 128, 128, 255);
    int Width = 1;
    float Alpha = 1.0f;
    e_brushType Type = e_brushType::not_set;
    e_imageBrush ImageBrushSelected = e_imageBrush::texture1;

    painter_brush() {}
    painter_brush(e_brushType value) : Type(value) {}
};




#endif // PAINTER_BRUSH_H
