#include "little_painter.h"
#include "graphic_helper_functions.h"


#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>

// ----------------------------------------------------------------------------------------------------

Little_Painter::Little_Painter(QWidget *parent) : QWidget(parent)
  , m_image(parent->size(), QImage::Format_RGBA8888_Premultiplied)
{
    setAttribute(Qt::WA_StaticContents);
    m_floatTexture.Size(parent->size());
    ClearImage();
    m_im.ResourcePath(QCoreApplication::applicationDirPath() + "/Resources");
    m_painterManager.Initialize();
    m_painterManager.BrushType(e_brushType::circleBrush);
    m_painterManager.Color(Qt::blue);
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
    m_painterManager.Color(newColor);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::setPenWidth(int newWidth)
{
    m_painterManager.Width(newWidth);
}

QColor Little_Painter::penColor() const
{
    return m_painterManager.Color();
}

int Little_Painter::penWidth() const
{
    return m_painterManager.Width();
}

void Little_Painter::SetBrushType(int value)
{
    m_painterManager.BrushType(static_cast<e_brushType>(value));
}

void Little_Painter::SetBrushCircle()
{
    m_painterManager.BrushType(e_brushType::circleBrush);
}

void Little_Painter::SetBrushTexture()
{
    m_painterManager.BrushType(e_brushType::textureBrush);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::ClearImage()
{
    m_image.fill(QColor(255, 255, 255, 255));
    m_modified = true;
    update();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->position().toPoint();
        m_measurePoint = event->position().toPoint();
        m_firstHit = true;
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
        //drawLineTo(event->position().toPoint());
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
    QRect area;

    m_painterManager.set_lastPoint(m_lastPoint);
    m_painterManager.set_measurePoint(m_measurePoint);
    m_painterManager.draw(painter, endPoint, area);

    if (m_floatTexture.SetImage(m_image, area) == false)
    {
        QMessageBox::warning(this, "Warning", "Image was not copied");
    }
    update(area);
    m_lastPoint = endPoint;
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::drawLineToBrush(int &pointsX, int &pointsY, int &posX, int &posY, int& radius, QPainter& painter)
{
    for (int x = 0, y = 0; x < pointsX; x++)
    {
        posX += x;
        ++y;
        if (y == pointsY)
        {
            y = 0;
            posY += y;
        }
        painter.drawEllipse(posX -  (radius / 2)
                            , posY -  (radius / 2)
                            , radius, radius);
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
