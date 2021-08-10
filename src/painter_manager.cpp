#include "painter_manager.h"
#include <QMessageBox>
#include "graphic_helper_functions.h"
#include <QDebug>

// -------------------------------------------------------------------------------------------------------------------------

painter_manager::painter_manager()
{
    m_brush.push_back(painter_brush(e_brushType::pen));
    m_brush.push_back(painter_brush(e_brushType::circleBrush));
    m_brush.push_back(painter_brush(e_brushType::textureBrush));
    m_brushType = e_brushType::textureBrush;

}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::draw(QPainter &p, const QPoint& endPoint, QRect& area)
{
    switch(m_brushType)
    {
        case e_brushType::pen: drawPen(p, endPoint, area); break;
        case e_brushType::circleBrush: drawBrush(p,endPoint, area); break;
        case e_brushType::textureBrush: drawTextureBrush(p, endPoint, area); break;
    }
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::BrushType(e_brushType value)
{
    m_brushType = value;
}

void painter_manager::ImageBrushSelect(e_imageBrush value)
{
    m_imageBrush = value;
}

// -------------------------------------------------------------------------------------------------------------------------

e_imageBrush painter_manager::ImageBrushSelected() const
{
    return m_imageBrush;
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::Color(QColor value)
{
    if (m_brushType != e_brushType::not_set)
    {
        m_brush[static_cast<int>(m_brushType)].Color = value;
        if (m_brushType == e_brushType::textureBrush)
        {
            painter_brush * pb = &m_brush[static_cast<int>(m_brushType)];
            QPainter p(&pb->Image);
            p.setCompositionMode(QPainter::CompositionMode::CompositionMode_SourceIn);
            p.fillRect(pb->Image.rect(), pb->Color);
            p.end();
        }

    }
}

QColor painter_manager::Color() const
{
    return m_brush[static_cast<int>(m_brushType)].Color;
}

// -------------------------------------------------------------------------------------------------------------------------

void painter_manager::Width(int value)
{
    m_brush[static_cast<int>(m_brushType)].Width = value;
}

// -------------------------------------------------------------------------------------------------------------------------

int painter_manager::Width() const
{
    return m_brush[static_cast<int>(m_brushType)].Width;
}

void painter_manager::set_lastPoint(QPoint &value)
{
    m_lastPoint = value;
}

void painter_manager::set_measurePoint(QPoint &value)
{
    m_measurePoint = value;
}

// =========================================================================================================================

void painter_manager::Initialize()
{
    /*for (int i = 0; i < m_brush.size(); i++)
    {
        m_brush[i].Color = QColor(128, 128, 128, 255);
        m_brush[i].Width = 1;

    }*/


    m_brush[static_cast<int>(e_brushType::textureBrush)].Width = 50;
    loadTexture();
    painter_brush * pb = &m_brush[static_cast<int>(e_brushType::textureBrush)];
    QPainter p(&pb->Image);
    p.setCompositionMode(QPainter::CompositionMode::CompositionMode_SourceIn);
    p.fillRect(pb->Image.rect(), pb->Color);
    p.end();
}

// =========================================================================================================================

void painter_manager::loadTexture()
{
    if (image_manager::loadAsResource(m_brush[static_cast<int>(e_brushType::textureBrush)].Image
                                      , m_brush[static_cast<int>(e_brushType::textureBrush)].ImageBrushSelected))
    {
        qInfo() << "image loaded";
    }
    else
    {
        QMessageBox::warning(nullptr, "Error", "Could not load texture.");
    }
}

// =========================================================================================================================

void painter_manager::drawPen(QPainter &p, const QPoint& endPoint, QRect& area)
{

}

// =========================================================================================================================

void painter_manager::drawBrush(QPainter &p, const QPoint& endPoint, QRect& area)
{
    painter_brush * pb = &m_brush[static_cast<int>(e_brushType::circleBrush)];
    // maximalna vzdialenost medzi dvoma bodmi
    // 1. bod kde sa naposledy vykreslila textura
    // 2. bod kde sa ma vykreslit nasledujuca textura
    int distanceCompare = std::ceil((static_cast<double>(pb->Width) / 2.0) / 4.0);

     /*
    if (m_lastPoint == endPoint)
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(m_penColor));
        int radius = m_penWidth + 2;
        painter.drawEllipse(endPoint.x() -  (radius / 2)
                            , endPoint.y() -  (radius / 2)
                            , radius, radius);
    }
    else
    {
        painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(m_lastPoint, endPoint);
    }
    */

    // nastavenie stetca
    QColor c(pb->Color);
    //if (m_painting) c.setAlphaF(0.25f);
    p.setPen(Qt::NoPen);
    QRadialGradient brushCircle(endPoint.x(), endPoint.y(), static_cast<float>(pb->Width) / 2.0f);
    brushCircle.setColorAt(0.0, c);
    //brushCircle.setColorAt(0.2, c);
    c.setAlphaF(0.0f);
    //brushCircle.setColorAt(0.8, c);
    brushCircle.setColorAt(1.0, c);
    p.setBrush(brushCircle);
    int radius = pb->Width + 0;

    // DISTANCE
    // vzdialenost medzi meratelnym bodom (m_measurePoint) a konecnym bodom (endPoint)
    float distance = 0.0f;
    ghf::distanceGet(distance, endPoint.x(), m_measurePoint.x(), endPoint.y(), m_measurePoint.y());

    //qInfo() << "distance = " << distance;

    // DRAWING
    if (m_lastPoint != endPoint)
    {
        int pointsX = std::abs(m_lastPoint.x() - endPoint.x());
        int pointsY = std::abs(m_lastPoint.y() - endPoint.y());
        int posX, posY;

        int loopMax = pointsX;
        if (pointsY > pointsX) { loopMax = pointsY; }
        float dimensionX = 0.0f
                , dimensionY = 0.0f;
        if (loopMax != 0)
        {
            dimensionX = static_cast<float>(endPoint.x() - m_lastPoint.x()) / loopMax;
            dimensionY = static_cast<float>(endPoint.y() - m_lastPoint.y()) / loopMax;

        }

        // ak sa taha stetcom pomaly, tak sa bude vykreslovat 1px kruh
        bool draw1px = loopMax >= 1 && loopMax <= 3;

        for (int a = 0; a < loopMax; a++)
        {
            posX = m_lastPoint.x() + std::ceilf(dimensionX * static_cast<float>(a));
            posY = m_lastPoint.y() + std::ceilf(dimensionY * static_cast<float>(a));

            ghf::distanceGet(distance, m_measurePoint.x(), posX, m_measurePoint.y(), posY);

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
                p.setBrush(brushCircle);
                p.drawEllipse(posX -  (radius / 2)
                                    , posY -  (radius / 2)
                                    , radius, radius);
                m_measurePoint = QPoint(posX, posY);
            }
        }

        //if (loopMax > 1)
         //   qInfo() << "-----------------------------";
    }
    else
    {
        //if (std::ceil(distance) >= distanceCompare || m_firstHit)
        {
            p.drawEllipse(endPoint.x() -  (radius / 2)
                                , endPoint.y() -  (radius / 2)
                                , radius, radius);
            m_measurePoint = endPoint;
            //m_firstHit = false;
            //qInfo() << "m_lastPoint == endPoint";
        }
    }

    //m_modified = true;

    int rad = (pb->Width / 2.0) + 2;
    area = QRect(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    m_lastPoint = endPoint;
}

// =========================================================================================================================

void painter_manager::drawTextureBrush(QPainter &p, const QPoint& endPoint, QRect& area)
{
    painter_brush * pb = &m_brush[static_cast<int>(e_brushType::textureBrush)];
    // maximalna vzdialenost medzi dvoma bodmi
    // 1. bod kde sa naposledy vykreslila textura
    // 2. bod kde sa ma vykreslit nasledujuca textura
    int distanceCompare = std::ceil((static_cast<double>(pb->Width) / 2.0) / 4.0);


    int radius = pb->Width + 2;

    // DISTANCE
    // vzdialenost medzi meratelnym bodom (m_measurePoint) a konecnym bodom (endPoint)
    float distance = 0.0f;
    ghf::distanceGet(distance, endPoint.x(), m_measurePoint.x(), endPoint.y(), m_measurePoint.y());

    // DRAWING
    if (m_lastPoint != endPoint)
    {
        int pointsX = std::abs(m_lastPoint.x() - endPoint.x());
        int pointsY = std::abs(m_lastPoint.y() - endPoint.y());
        int posX, posY;

        int loopMax = pointsX;
        if (pointsY > pointsX) { loopMax = pointsY; }
        float dimensionX = 0.0f
                , dimensionY = 0.0f;
        if (loopMax != 0)
        {
            dimensionX = static_cast<float>(endPoint.x() - m_lastPoint.x()) / loopMax;
            dimensionY = static_cast<float>(endPoint.y() - m_lastPoint.y()) / loopMax;

        }

        // ak sa taha stetcom pomaly, tak sa bude vykreslovat 1px kruh
        bool draw1px = loopMax >= 1 && loopMax <= 3;

        for (int a = 0; a < loopMax; a++)
        {
            posX = m_lastPoint.x() + std::ceilf(dimensionX * static_cast<float>(a));
            posY = m_lastPoint.y() + std::ceilf(dimensionY * static_cast<float>(a));


            ghf::distanceGet(distance, m_measurePoint.x(), posX, m_measurePoint.y(), posY);

            if (std::ceil(distance) >= distanceCompare || draw1px)
            {
                p.drawImage(QPoint(posX -  (radius / 2), posY -  (radius / 2)), pb->Image);
                m_measurePoint = QPoint(posX, posY);
            }
        }
    }
    else
    {

        p.drawImage(QPoint(endPoint.x() -  (radius / 2), endPoint.y() -  (radius / 2)), pb->Image);
        m_measurePoint = endPoint;
        //m_firstHit = false;
    }

    //m_modified = true;

    int rad = (pb->Width / 2.0) + 2;
    area = QRect(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    m_lastPoint = endPoint;
}

// =========================================================================================================================

