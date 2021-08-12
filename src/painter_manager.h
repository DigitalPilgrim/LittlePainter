#ifndef PAINTER_MANAGER_H
#define PAINTER_MANAGER_H

#include "painter_brush.h"
#include <vector>
#include <QPoint>
#include "floattexture.h"

class painter_manager
{
    std::vector<painter_brush> m_brush;
    e_brushType m_brushType = e_brushType::textureBrush;
    e_imageBrush m_imageBrush = e_imageBrush::texture1;
    QPoint m_lastPoint;
    QPoint m_measurePoint;

    FloatTexture * m_floatTexture = nullptr;
public:
    painter_manager();

    void setFloatTexture(FloatTexture * ft) { m_floatTexture = ft; }

    void draw(QPainter& p, const QPoint& endPoint, QRect& area);

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
private:
    void setImageColor();
    void drawPen(QPainter& p, const QPoint& endPoint, QRect& area);
    void drawBrush(QPainter& p, const QPoint& endPoint, QRect& area);
    void drawTextureBrush(QPainter& p, const QPoint& endPoint, QRect& area);
    void loadTexture();
};

#endif // PAINTER_MANAGER_H
