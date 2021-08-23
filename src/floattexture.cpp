#include "floattexture.h"
#include <QtDebug>

FloatTexture::FloatTexture()
{

}

void FloatTexture::Size(const QSize size)
{
    yx.resize(size.height());
    for (int y = 0; y < size.height(); y++)
    {
        yx[y].resize(size.width());
        for (int x = 0; x < size.width(); x++)
        {
            yx[y][x] = { 1.0, 1.0, 1.0, 1.0 };
        }
    }
    m_size = size;
}

QSize FloatTexture::Size() const
{
    return m_size;
}

void FloatTexture::Clear(bool makeTransparent)
{
    float a = 1.0f;
    if (makeTransparent) a = 0.0f;
    for (int y = 0; y < yx.size(); y++)
    {
        for (int x = 0; x < yx[0].size(); x++)
        {
            yx[y][x] = { 1.0, 1.0, 1.0, a };
        }
    }
}

int FloatTexture::Width() const
{
    return  m_size.width(); //xy.size();
}

int FloatTexture::Height() const
{
    return m_size.height(); // xy[0].size();
}

bool FloatTexture::SetPixelColor(const QColor &c, const QPoint &pos)
{
    bool ok = false;

    if (pos.y() < yx.size() && pos.x() < yx[0].size())
    {
        float   r = 0.0
                , g = 0.0
                , b = 0.0
                , a =0.0;
        c.getRgbF(&r, &g, &b, &a);
        yx[pos.y()][pos.x()] = { r, g, b, a };
        ok = true;
    }

    return ok;
}

bool FloatTexture::SetPixelColor(const float &r, const float &g, const float &b, const float &a, const QPoint &pos)
{
    bool ok = false;

    if (pos.y() < yx.size() && pos.x() < yx[0].size())
    {
        yx[pos.y()][pos.x()] = { r, g, b, a };
        ok = true;
    }

    return ok;
}

bool FloatTexture::SetImage(const QImage &image)
{
    bool ok = true;

    QColor c;

    if (image.size() != Size())
    {
        Size(image.size());
        if (yx.size() < image.size().height() || yx[0].size() < image.size().width())
        {
            ok = false;
        }
    }
    if (ok)
    {
        for (int y = 0; y < image.height(); y++)
        {
            for (int x = 0; x < image.width(); x++)
            {
                c = image.pixelColor(x, y);
                SetPixelColorInternal(c, QPoint(x, y));
                //ok = SetPixelColor(c, QPoint(x, y));
                //if (ok == false) break;
            }
        }
    }

    return ok;
}

bool FloatTexture::SetImage(const QImage &image, const QRect &area, bool imageAreaPosZero, bool add)
{
   /* qInfo() << "SetImage(const QImage &image, const QRect &area) | image.size() "
            << "w = " << image.size().width()
            << ", h = " << image.size().height();*/
    bool ok = true;
    QColor c;

    int areaX = area.left()
            , areaXmax = area.right()
            , areaY = area.top()
            , areaYmax = area.bottom();

    /*qInfo() << "-- SetImage(image, area imageAreaPosZero) | "
            << "areaX = " << areaX
            << "areaY = " << areaY
            << " | areaXmax = " << areaXmax
            << "areaYmax = " << areaYmax;*/

    if (areaX < 0) areaX = 0;
    if (areaXmax > m_size.width()) areaXmax = m_size.width();
    if (areaY < 0) areaY = 0;
    if (areaYmax > m_size.height()) areaYmax = m_size.height();

    /*qInfo() << "-- SetImage(image, area imageAreaPosZero) | "
            << "areaX = " << areaX
            << "areaY = " << areaY
            << " | areaXmax = " << areaXmax
            << "areaYmax = " << areaYmax;
    qInfo() << "------------------------------------------";*/

    int iX = areaX
            , iY = areaY
            , iYreset = areaY
            , iXreset = areaX;

    if (imageAreaPosZero)
    {
        iX = 0;
        iY = 0;
        iYreset = 0;
        iXreset = 0;
    }

    //qInfo() << "iX = " << iX << ", iY = " << iY;
    //qInfo() << "areaX = " << areaX << ", areaY = " << areaY;

    if (add)
    {
        mf_Color = &FloatTexture::AddPixelColorInternal;
    }
    else
    {
        mf_Color = &FloatTexture::SetPixelColorInternal;
    }

    for (int y = areaY; y < areaYmax; y++)
    {
        for (int x = areaX; x < areaXmax; x++)
        {
            c = image.pixelColor(iX, iY);
            //SetPixelColorInternal(c, QPoint(x, y));
            (this->*mf_Color)(c, QPoint(x, y));
            ++iX;
        }
        ++iY;
        iX = iXreset;
    }
    return ok;
}

bool FloatTexture::AddImage(const QImage &image, const QRect &area, bool imageAreaPosZero)
{
    return SetImage(image, area, imageAreaPosZero, true);
}

bool FloatTexture::GetImage(QImage &image)
{
    bool ok = true;
    QColor c;
    FRGBA * fc = nullptr;
    if (yx.size() < image.size().height() && yx[0].size() < image.size().width())
    {
        ok = false;
    }
    else
    {
        for (int y = 0; y < image.height(); y++)
        {
            for (int x = 0; x < image.width(); x++)
            {
                fc = &yx[y][x];
                c.setRgbF(fc->R, fc->G, fc->B, fc->A);
                image.setPixelColor(x, y, c);
            }
        }
    }

    return ok;
}

bool FloatTexture::GetImage(QImage &image, const QRect &area)
{
    bool ok = true;
    QColor c;
    FRGBA * fc = nullptr;

    int areaX = area.left()
            , areaXmax = area.right()
            , areaY = area.top()
            , areaYmax = area.bottom();

    if (areaX < 0) areaX = 0;
    if (areaXmax > m_size.width()) areaXmax = m_size.width();
    if (areaY < 0) areaY = 0;
    if (areaYmax > m_size.height()) areaYmax = m_size.height();

    for (int y = areaY; y < areaYmax; y++)
    {
        for (int x = areaX; x < areaXmax; x++)
        {
            fc = &yx[y][x];
            //c.setRgbF(fc->R, fc->G, fc->B, fc->A);
            c.setRgb(static_cast<int>(fc->R * 255.f)
                     , static_cast<int>(fc->G * 255.f)
                     , static_cast<int>(fc->B * 255.f)
                     , static_cast<int>(fc->A * 255.f));
            image.setPixelColor(x, y, c);
        }
    }
    /*for (int x = area.left(); x < area.right(); x++)
    {
        for (int y = area.top(); y < area.bottom(); y++)
        {
            fc = &yx[y][x];
            c.setRgbF(fc->R, fc->G, fc->B, fc->A);
            image.setPixelColor(x, y, c);
        }
    }*/
    return ok;
}

void FloatTexture::DrawImageToImage(QImage &drawTo, const QImage &drawFrom, const QRect &area)
{

}

void FloatTexture::ClearImage(QImage &image, const QRect &area)
{
    QColor c;
    int areaX = area.left()
            , areaXmax = area.right()
            , areaY = area.top()
            , areaYmax = area.bottom();

    if (areaX < 0) areaX = 0;
    if (areaXmax > m_size.width()) areaXmax = m_size.width();
    if (areaY < 0) areaY = 0;
    if (areaYmax > m_size.height()) areaYmax = m_size.height();

    for (int y = areaY; y < areaYmax; y++)
    {
        for (int x = areaX; x < areaXmax; x++)
        {
            c.setRgb(255, 255, 255, 0);
            image.setPixelColor(x, y, c);
        }
    }
}

/*bool FloatTexture::Draw(const DrawArgs &drawArgs)
{
    return false;
}*/

void FloatTexture::SetPixelColorInternal(const QColor &c, const QPoint &pos)
{
    float   r = 0.0
            , g = 0.0
            , b = 0.0
            , a =0.0;
    c.getRgbF(&r, &g, &b, &a);
    yx[pos.y()][pos.x()] = { r, g, b, a };
}

void FloatTexture::AddPixelColorInternal(const QColor &c, const QPoint &pos)
{
    float   r = 0.0
            , g = 0.0
            , b = 0.0
            , a =0.0;
    c.getRgbF(&r, &g, &b, &a);
    FRGBA & fc = yx[pos.y()][pos.x()];
    fc.R = fc.R * (1.f - a) + r * a;
    fc.G = fc.G * (1.f - a) + g * a;
    fc.B = fc.B * (1.f - a) + b * a;
}
