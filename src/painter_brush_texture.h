#ifndef PAINTER_BRUSH_TEXTURE_H
#define PAINTER_BRUSH_TEXTURE_H

#include "painter_brush.h"

class painter_brush_texture : public painter_brush
{
    bool m_alphaSet = false;
public:
    painter_brush_texture();

    void draw(const DrawManagerArgs& args) override;
    void setImageColor(const QColor &value) override;
    void setImageAlpha(const float &alpha) override;
private:
    void createOriginalAlpha(bool recreate = false);
};

#endif // PAINTER_BRUSH_TEXTURE_H
