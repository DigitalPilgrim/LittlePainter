#include "painter_brush_texture.h"
#include "graphic_helper_functions.h"
#include <QDebug>

painter_brush_texture::painter_brush_texture()
    : painter_brush(e_brushType::textureBrush)
{
    /* ======================================================================
     *  DRAW TEXTURE
     *  ======================================================================
     * */
}

void painter_brush_texture::draw(const DrawManagerArgs &args)
{
    painter_brush * pb = this;

    if (args.MeasurePoint == nullptr || args.LastPoint == nullptr)
    {
        qInfo() << "missing arguments in painter brush texture !";
        return;
    }

    // maximalna vzdialenost medzi dvoma bodmi
    // 1. bod kde sa naposledy vykreslila textura
    // 2. bod kde sa ma vykreslit nasledujuca textura
    int distanceCompare = std::ceil((static_cast<double>(pb->Width) / 2.0) / 4.0);


    int radius = pb->Width + 2;

    // DISTANCE
    // vzdialenost medzi meratelnym bodom (m_measurePoint) a konecnym bodom (endPoint)
    float distance = 0.0f;
    ghf::distanceGet(distance, args.EndPoint->x(), args.MeasurePoint->x(), args.EndPoint->y(), args.MeasurePoint->y());

    // DRAWING
    //args.Painter->setCompositionMode(QPainter::CompositionMode_Source);
    args.Painter->setOpacity(pb->Alpha);
    if (*args.LastPoint != *args.EndPoint)
    {
        int pointsX = std::abs(args.LastPoint->x() - args.EndPoint->x());
        int pointsY = std::abs(args.LastPoint->y() - args.EndPoint->y());
        int posX, posY;

        int loopMax = pointsX;
        if (pointsY > pointsX) { loopMax = pointsY; }
        float dimensionX = 0.0f
                , dimensionY = 0.0f;
        if (loopMax != 0)
        {
            dimensionX = static_cast<float>(args.EndPoint->x() - args.LastPoint->x()) / loopMax;
            dimensionY = static_cast<float>(args.EndPoint->y() - args.LastPoint->y()) / loopMax;

        }

        // ak sa taha stetcom pomaly, tak sa bude vykreslovat 1px kruh
        bool draw1px = loopMax >= 1 && loopMax <= 3;

        for (int a = 0; a < loopMax; a++)
        {
            posX = args.LastPoint->x() + std::ceilf(dimensionX * static_cast<float>(a));
            posY = args.LastPoint->y() + std::ceilf(dimensionY * static_cast<float>(a));


            ghf::distanceGet(distance, args.MeasurePoint->x(), posX, args.MeasurePoint->y(), posY);

            // -------------- Poznamka -----------------------------------------------------------------------
            // v Debug mode, ak je 1px nastaveny na stalo (teda sa neoptimalizuje) pri rychlom pohybe stetcom
            // uz nestiha vykreslovat, ale v Release mode to ide svizne stale.
            // -----------------------------------------------------------------------------------------------

            //if (std::ceil(distance) >= distanceCompare || draw1px)
            {
                args.Painter->drawImage(QPoint(posX -  (radius / 2), posY -  (radius / 2)), pb->Image);
                *args.MeasurePoint = QPoint(posX, posY);
            }
        }
    }
    else
    {

        args.Painter->drawImage(QPoint(args.EndPoint->x() -  (radius / 2), args.EndPoint->y() -  (radius / 2)), pb->Image);
        *args.MeasurePoint = *args.EndPoint;
        //m_firstHit = false;
    }

    //args.Painter->setOpacity(1.0f);

    int rad = (pb->Width / 2.0) + 2;
    *args.Area = QRect(QRect(*args.LastPoint, *args.EndPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    *args.LastPoint = *args.EndPoint;
}

// ================================================================================================================

void painter_brush_texture::setImageColor(const QColor &value)
{
    painter_brush * pb = this;
    pb->Color = value;
    QColor cFromImage;
    QColor cToImage;
    QSize imageSize = pb->Image.size();
    for (int x = 0; x < imageSize.width(); x++)
    {
        for (int y = 0; y < imageSize.height(); y++)
        {
            cFromImage = pb->Image.pixelColor(x, y);
            cToImage = pb->Color;
            cToImage.setAlpha(cFromImage.alpha()); // std::ceil(cFromImage.alpha() * pb->Alpha)
            pb->Image.setPixelColor(x, y, cToImage);
        }
    }

    //createOriginalAlpha(true);

}

void painter_brush_texture::setImageAlpha(const float &alpha)
{
    /*createOriginalAlpha();

    QColor cFromImage;
    QColor cToImage;
    QSize imageSize = Image.size();
    for (int y = 0; y < imageSize.height(); y++)
    {
        for (int x = 0; x < imageSize.width(); x++)
        {
            cFromImage = Image.pixelColor(x, y);
            cToImage = Color;
            cToImage.setAlpha(OriginalImageAlpha[y][x] * alpha);
            Image.setPixelColor(x, y, cToImage);
        }
    }*/
}

void painter_brush_texture::createOriginalAlpha(bool recreate)
{
    /*if ((!m_alphaSet || recreate) && Image.size().height() > 0)
    {
        QColor aFromImage;
        QSize imageSize = Image.size();

        if (OriginalImageAlpha.size() < imageSize.height())
        {
            OriginalImageAlpha.resize(imageSize.height());
            for (int y = 0; y < imageSize.height(); y++)
            {
                OriginalImageAlpha[y].resize(imageSize.width());
            }
        }
        for (int y = 0; y < imageSize.height(); y++)
        {
            for (int x = 0; x < imageSize.width(); x++)
            {
                aFromImage = Image.pixelColor(x, y);
                OriginalImageAlpha[y][x] = aFromImage.alpha();
            }
        }
        m_alphaSet = true;
    }*/
}
