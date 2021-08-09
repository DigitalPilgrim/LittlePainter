#ifndef FLOATTEXTURE_H
#define FLOATTEXTURE_H

#include <QSize>
#include <QPoint>
#include <QColor>
#include <QImage>
#include <vector>

struct FRGBA
{
    float R = 0.0;
    float G = 0.0;
    float B = 0.0;
    float A = 0.0;

    FRGBA() {}
    FRGBA(const float r, const float g, const float b, const float a)
        : R(r), G(g), B(b), A(a) {}
};

class FloatTexture
{
    std::vector<std::vector<FRGBA>> xy;
    QSize m_size;
public:
    FloatTexture();

    void Size(const QSize size);
    QSize Size() const;

    int Width() const;
    int Height() const;

    bool SetPixelColor(const QColor& c, const QPoint& pos);
    bool SetPixelColor(const float& r, const float& g, const float& b, const float& a, const QPoint& pos);

    bool SetImage(const QImage& image);
    bool SetImage(const QImage& image, const QRect& area);
    bool GetImage(QImage& image);
    bool GetImage(QImage& image, const QRect& area);

private:

    void SetPixelColorInternal(const QColor& c, const QPoint& pos);


};

#endif // FLOATTEXTURE_H
