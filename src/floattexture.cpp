#include "floattexture.h"


FloatTexture::FloatTexture()
{

}

void FloatTexture::Size(const QSize size)
{
    yx.resize(size.height());
    for (int x = 0; x < size.height(); x++)
    {
        yx[x].resize(size.width());
    }
    m_size = size;
}

QSize FloatTexture::Size() const
{
    return m_size;
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
        if (yx.size() < image.size().height() && yx[0].size() < image.size().width())
        {
            ok = false;
        }
    }
    if (ok)
    {
        for (int x = 0; x < image.width(); x++)
        {
            for (int y = 0; y < image.height(); y++)
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

bool FloatTexture::SetImage(const QImage &image, const QRect &area)
{
    bool ok = true;
    QColor c;

    int areaX = area.left()
            , areaXmax = area.right()
            , areaY = area.top()
            , areaYmax = area.bottom();

    if (areaX < 0) areaX = 0;
    if (areaXmax > m_size.width()) areaXmax = m_size.width();
    if (areaY < 0) areaY = 0;
    if (areaYmax > m_size.height()) areaYmax = m_size.height();

    for (int x = areaX; x < areaXmax; x++)
    {
        for (int y = areaY; y < areaYmax; y++)
        {
            c = image.pixelColor(x, y);
            SetPixelColorInternal(c, QPoint(x, y));
        }
    }
    return ok;
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
        for (int x = 0; x < image.width(); x++)
        {
            for (int y = 0; y < image.height(); y++)
            {
                fc = &yx[x][y];
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

    for (int x = area.left(); x < area.right(); x++)
    {
        for (int y = area.top(); y < area.bottom(); y++)
        {
            fc = &yx[x][y];
            c.setRgbF(fc->R, fc->G, fc->B, fc->A);
            image.setPixelColor(x, y, c);
        }
    }
    return ok;
}

void FloatTexture::SetPixelColorInternal(const QColor &c, const QPoint &pos)
{
    float   r = 0.0
            , g = 0.0
            , b = 0.0
            , a =0.0;
    c.getRgbF(&r, &g, &b, &a);
    yx[pos.y()][pos.x()] = { r, g, b, a };
    //FRGBA test = xy[pos.x()][pos.y()];
    //r = test.R;
}
