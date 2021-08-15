#include "painter_brush_circle.h"
#include "graphic_helper_functions.h"
#include <QDebug>

painter_brush_circle::painter_brush_circle()
    : painter_brush(e_brushType::circleBrush)
{
    /* ======================================================================
     *  DRAW CIRCLE
     *  ======================================================================
     * */
}

void painter_brush_circle::draw(const DrawManagerArgs &args)
{
    painter_brush * pb = this;

    if (args.MeasurePoint == nullptr || args.LastPoint == nullptr)
    {
        qInfo() << "missing arguments in painter brush circle !";
        return;
    }

    // maximalna vzdialenost medzi dvoma bodmi
    // 1. bod kde sa naposledy vykreslila textura
    // 2. bod kde sa ma vykreslit nasledujuca textura
    int distanceCompare = std::ceil((static_cast<double>(pb->Width) / 2.0) / 4.0);

     /*
    if (*args.LastPoint == endPoint)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(m_penColor));
        int radius = m_penWidth + 2;
        painter.drawEllipse(args.EndPoint->x() -  (radius / 2)
                            , args.EndPoint->y() -  (radius / 2)
                            , radius, radius);
    }
    else
    {
        painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(*args.LastPoint, endPoint);
    }
    */

    // nastavenie stetca
    args.Painter->setOpacity(pb->Alpha);
    QColor c(pb->Color);
    //if (m_painting) c.setAlphaF(0.25f);
    args.Painter->setPen(Qt::NoPen);
    QRadialGradient brushCircle(args.EndPoint->x(), args.EndPoint->y(), static_cast<float>(pb->Width) / 2.0f);
    brushCircle.setColorAt(0.0, c);
    //brushCircle.setColorAt(0.2, c);
    c.setAlphaF(0.0f);
    //brushCircle.setColorAt(0.8, c);
    brushCircle.setColorAt(1.0, c);
    args.Painter->setBrush(brushCircle);
    int radius = pb->Width + 0;


    // DISTANCE
    // vzdialenost medzi meratelnym bodom (m_measurePoint) a konecnym bodom (endPoint)
    float distance = 0.0f;
    ghf::distanceGet(distance, args.EndPoint->x(), args.MeasurePoint->x(), args.EndPoint->y(), args.MeasurePoint->y());

    //qInfo() << "distance = " << distance;

    // DRAWING
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

            if (std::ceil(distance) >= distanceCompare || draw1px)
            {
                //
               if (draw1px)
                {
                    c.setAlphaF(0.25f);
                    brushCircle.setColorAt(0.0, c);
                    c.setAlphaF(0.0f);
                    brushCircle.setColorAt(1.0, c);
                }
                brushCircle.setCenter(posX, posY);
                brushCircle.setCenterRadius(static_cast<float>(pb->Width) / 2.0f);
                brushCircle.setFocalPoint(posX, posY);
                brushCircle.setFocalRadius(0);
                args.Painter->setBrush(brushCircle);
                args.Painter->drawEllipse(posX -  (radius / 2)
                                    , posY -  (radius / 2)
                                    , radius, radius);

                *args.MeasurePoint = QPoint(posX, posY);

            }
        }

        //if (loopMax > 1)
         //   qInfo() << "-----------------------------";
    }
    else
    {
        //if (std::ceil(distance) >= distanceCompare || m_firstHit)
        {
            args.Painter->drawEllipse(args.EndPoint->x() -  (radius / 2)
                                , args.EndPoint->y() -  (radius / 2)
                                , radius, radius);
            *args.MeasurePoint = *args.EndPoint;
            //m_firstHit = false;
            //qInfo() << "*args.LastPoint == endPoint";
        }
    }

    //m_modified = true;
    args.Painter->setOpacity(1.0f);

    int rad = (pb->Width / 2.0) + 2;
    //QRect r = QRect(QRect(*args.LastPoint, *args.EndPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    *args.Area = QRect(QRect(*args.LastPoint, *args.EndPoint).normalized().adjusted(-rad, -rad, +rad, +rad));//QRect(r.left(), r.top(), r.width(), r.height()); //
    *args.LastPoint = *args.EndPoint;
}
