#ifndef PAINTER_MANAGER_H
#define PAINTER_MANAGER_H

#include "painter_brush.h"
#include <vector>
#include <QPoint>
#include "floattexture.h"


class painter_manager
{
    int BRUSH_SELECTED = 0;
    const int BRUSH_CIRCLE = 0;//static_cast<int>(e_brushType::circleBrush);
    const int BRUSH_TEXTURE = 1; //static_cast<int>(e_brushType::textureBrush);
    std::vector<painter_brush *> m_brushes;
    e_brushType m_brushType = e_brushType::textureBrush;
    e_imageBrush m_imageBrush = e_imageBrush::texture1;

    FloatTexture * m_floatTexture = nullptr;
public:
    painter_manager();
    ~painter_manager();

    void setFloatTexture(FloatTexture * ft) { m_floatTexture = ft; }

    void draw(const DrawManagerArgs &args); //QPainter& p, const QPoint& endPoint, QRect& area

    void BrushType(e_brushType value);
    void ImageBrushSelect(e_imageBrush value);
    e_imageBrush ImageBrushSelected() const;
    void Color(QColor value);
    QColor Color() const;
    void BrushTransparency(const float & value);
    float BrushTransparency() const;
    void Width(int value);
    int Width() const;
    void set_lastPoint(QPoint & value);
    void set_measurePoint(QPoint& value);

    void Initialize();

    painter_brush * GetBrush();
private:
    void loadTexture();
};

#endif // PAINTER_MANAGER_H
