#ifndef PAINTER_BRUSH_CIRCLE_H
#define PAINTER_BRUSH_CIRCLE_H

#include "painter_brush.h"

class painter_brush_circle : public painter_brush
{
public:
    painter_brush_circle();

    void draw(const DrawManagerArgs& args) override;
};

#endif // PAINTER_BRUSH_CIRCLE_H
