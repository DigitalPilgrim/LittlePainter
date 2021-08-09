#include "floattexture.h"

FloatTexture::FloatTexture()
{

}

void FloatTexture::Size(const QSize size)
{
    xy.resize(size.width());
    for (int x = 0; x < size.width(); x++)
    {
        xy[x].resize(size.height());
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

    if (pos.x() < xy.size() && pos.y() < xy[0].size())
    {
        float   r = 0.0
                , g = 0.0
                , b = 0.0
                , a =0.0;
        c.getRgbF(&r, &g, &b, &a);
        xy[pos.x()][pos.y()] = { r, g, b, a };
        ok = true;
    }

    return ok;
}

bool FloatTexture::SetPixelColor(const float &r, const float &g, const float &b, const float &a, const QPoint &pos)
{
    bool ok = false;

    if (pos.x() < xy.size() && pos.y() < xy[0].size())
    {
        xy[pos.x()][pos.y()] = { r, g, b, a };
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
        if (xy.size() < image.size().width() && xy[0].size() < image.size().height())
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
    bool ok = false;
    return ok;
}

bool FloatTexture::GetImage(QImage &image)
{
    bool ok = true;
    QColor c;
    FRGBA * fc = nullptr;
    if (xy.size() < image.size().width() && xy[0].size() < image.size().height())
    {
        ok = false;
    }
    else
    {
        for (int x = 0; x < image.width(); x++)
        {
            for (int y = 0; y < image.height(); y++)
            {
                fc = &xy[x][y];
                c.setRgbF(fc->R, fc->G, fc->B, fc->A);
                image.setPixelColor(x, y, c);
            }
        }
    }

    return ok;
}

bool FloatTexture::GetImage(QImage &image, const QRect &area)
{
    bool ok = false;
    return ok;
}

void FloatTexture::SetPixelColorInternal(const QColor &c, const QPoint &pos)
{
    float   r = 0.0
            , g = 0.0
            , b = 0.0
            , a =0.0;
    c.getRgbF(&r, &g, &b, &a);
    xy[pos.x()][pos.y()] = { r, g, b, a };
    //FRGBA test = xy[pos.x()][pos.y()];
    //r = test.R;
}
