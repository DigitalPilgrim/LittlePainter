#include "little_painter.h"


#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>

// ----------------------------------------------------------------------------------------------------

Little_Painter::Little_Painter(QWidget *parent) : QWidget(parent)
  , m_image(parent->size(), QImage::Format_RGBA8888_Premultiplied)
{
    setAttribute(Qt::WA_StaticContents);
    m_floatTexture.Size(parent->size());
    ClearImage();
}

// ----------------------------------------------------------------------------------------------------

bool Little_Painter::openImage(const QString &fileName)
{
    QImage img;
    if (!img.load(fileName)) return false;

    QSize nSize = img.size().expandedTo(size());
    resizeImage(&img, nSize);
    m_image = img;
    m_modified = false;
    update();
    return true;
}

// ----------------------------------------------------------------------------------------------------

bool Little_Painter::saveImage(const QString &fileName, const char* fileFormat)
{
    QImage img = m_image;
    resizeImage(&img, size());
    if (img.save(fileName, fileFormat))
    {
        m_modified = false;
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::setPenColor(const QColor &newColor)
{
    m_penColor = newColor;
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::setPenWidth(int newWidth)
{
    m_penWidth = newWidth;
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::ClearImage()
{
    m_image.fill(QColor(255, 255, 255, 255));
    //m_image.fill(qRgb(255, 255, 255));
    m_modified = true;
    update();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->position().toPoint();
        drawLineTo(event->position().toPoint());
        m_painting = true;
    }
}

void Little_Painter::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_painting)
    {
        drawLineTo(event->position().toPoint());
    }
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_painting)
    {
        drawLineTo(event->position().toPoint());
        drawEnd();
        m_painting = false;
    }
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dRect = event->rect();
    painter.drawImage(dRect, m_image, dRect);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::resizeEvent(QResizeEvent *event)
{
    if (width() > m_image.width() || height() > m_image.height())
    {
        int nWidth = qMax(width() + 128, m_image.width());
        int nHeight = qMax(height() + 128, m_image.height());
        resizeImage(&m_image, QSize(nWidth, nHeight));
        m_floatTexture.Size(m_image.size());
        update();
    }
    QWidget::resizeEvent(event);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&m_image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
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


    m_modified = true;

    int rad = (m_penWidth / 2.0) + 2;
    update(QRect(m_lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    m_lastPoint = endPoint;
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::drawEnd()
{
    if (m_floatTexture.SetImage(m_image) == false)
    {
        QMessageBox::warning(this, "Warning", "Image was not copied");
    }
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize) return;

    //QImage nImage(newSize, QImage::Format_RGB32);
    QImage nImage(newSize, QImage::Format_RGBA8888_Premultiplied);
    nImage.fill(QColor(255, 255, 255, 255));
    //nImage.fill(qRgb(255, 255, 255));
    QPainter painter(&nImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = nImage;
}
