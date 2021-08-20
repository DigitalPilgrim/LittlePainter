#include "little_painter.h"
#include "graphic_helper_functions.h"
#include "file_manager.h"


#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>

#include "undo_redo_system.h"
#include "painter_brush_debug.h"

// ----------------------------------------------------------------------------------------------------

Little_Painter::Little_Painter(QWidget *parent) : QWidget(parent)
  , m_image(parent->size(), QImage::Format_RGBA8888_Premultiplied)
  , m_imageDraw(parent->size(), QImage::Format_RGBA8888_Premultiplied)
  //, m_imageEmpty(parent->size(), QImage::Format_RGBA8888_Premultiplied)
  , m_imageRender(&m_image)
{
    setAttribute(Qt::WA_StaticContents);

    undo_redo_system::setFileCachePath(QCoreApplication::applicationDirPath() + "/Cache/");
    m_floatTexture.Size(parent->size());
    qInfo() << "m_image size | w = " << m_image.size().width() << " h = " << m_image.size().height();
    ClearImage();
    m_im.ResourcePath(QCoreApplication::applicationDirPath() + "/Resources");
    m_painterManager.Initialize();
    m_painterManager.BrushType(e_brushType::circleBrush);
    m_painterManager.Color(Qt::blue);


    /*UndoRedoArgs ura(ghf::AreaPixels(0, 0, m_image.size().width(), m_image.height()), &m_image, &m_floatTexture);
    undo_redo_system::set(ura, true);*/
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::Undo()
{
    qInfo() << "UNDO - BEGIN ----------------------------------";
    using urs = undo_redo_system;
    //UndoRedoArgs ura(&m_image, &m_floatTexture);
    ghf::AreaPixels area;
    UndoRedoSpecialArgs ura(m_imageRender, area);
    //UndoRedoSpecialArgs ura(&m_image, area);
    if (urs::undoS(ura))
    {
       /* qInfo() << "undo done | Area x = " << ura.Area.Left
                << ", y = " << ura.Area.Top
                << ", w = " << ura.Area.Right
                << ", h = " << ura.Area.Bottom;*/

        drawBoundingBox(ura.Area);
        update(QRect(ura.Area.Left, ura.Area.Top, ura.Area.Right, ura.Area.Bottom));
        qInfo() << "UNDO - END ----------------------------------";
    }
    qInfo() << "---------------------------------------------";
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::Redo()
{
    qInfo() << "REDO - BEGIN ----------------------------------";
    using urs = undo_redo_system;
    //UndoRedoArgs ura(&m_image, &m_floatTexture);
    ghf::AreaPixels area;
    UndoRedoSpecialArgs ura(m_imageRender, area);
    //UndoRedoSpecialArgs ura(&m_image, area);
    if (urs::redoS(ura))
    {
        /*qInfo() << "undo done | Area x = " << ura.Area.Left
                << ", y = " << ura.Area.Top
                << ", w = " << ura.Area.Right
                << ", h = " << ura.Area.Bottom;*/
        //drawBoundingBox(ura.Area);
        update(QRect(ura.Area.Left, ura.Area.Top, ura.Area.Right, ura.Area.Bottom));
        qInfo() << "REDO - END ----------------------------------";
    }
    qInfo() << "---------------------------------------------";
}

// ----------------------------------------------------------------------------------------------------

bool Little_Painter::openImage(/*const QString &fileName*/)
{
    QImage img;
    e_file_state fs = file_manager::openImage(img, this);
    if (fs == e_file_state::opened)
    {
        /*QSize nSize = img.size().expandedTo(size());
        resizeImage(&img, nSize);*/
        QImage toFloat = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        m_floatTexture.SetImage(toFloat);
        *m_imageRender = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied);
        m_modified = false;
        update();

        undo_redo_system::setS(UndoRedoSpecialSetIniArgs(m_imageRender->size()));
        undo_redo_system::setSBegin(UndoRedoSpecialBeginArgs(m_imageRender));
        return true;
    }
    else if (fs == e_file_state::error) return false;
    return true;
}

// ----------------------------------------------------------------------------------------------------

bool Little_Painter::saveImage(/*const QString &fileName, const char* fileFormat*/)
{
   // QImage img = m_image;
    QImage img(m_floatTexture.Size(), QImage::Format_ARGB32_Premultiplied);// = m_image;
    if (!m_floatTexture.GetImage(img))
    {
        return false;
    }
    //resizeImage(&img, size());
    e_file_state fs = file_manager::saveImage(img, this);
    if (fs == e_file_state::saved || fs == e_file_state::closed)
    {
        m_modified = false;
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------------------------------------

bool Little_Painter::saveImageAs()
{
    qInfo() << "image size | w = " << m_floatTexture.Size().width() << " h = " << m_floatTexture.Size().height();
    QImage img(m_floatTexture.Size(), QImage::Format_ARGB32_Premultiplied);// = m_image;
    if (!m_floatTexture.GetImage(img))
    {
        return false;
    }
    //resizeImage(&img, size());
    m_modified = false;
    e_file_state fs = file_manager::saveImage(img, true, this);
    if (fs == e_file_state::saved || fs == e_file_state::closed) return true;
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

// ----------------------------------------------------------------------------------------------------

QColor Little_Painter::penColor() const
{
    return m_painterManager.Color();
}

// ----------------------------------------------------------------------------------------------------

int Little_Painter::penWidth() const
{
    return m_painterManager.Width();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::BrushTransparency(const float &value)
{
    m_painterManager.BrushTransparency(value);
}

// ----------------------------------------------------------------------------------------------------

float Little_Painter::BrushTransparency() const
{
    return m_painterManager.BrushTransparency();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::SetBrushType(int value)
{
    m_painterManager.BrushType(static_cast<e_brushType>(value));
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::SetBrushCircle()
{
    m_painterManager.BrushType(e_brushType::circleBrush);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::SetBrushTexture()
{
    m_painterManager.BrushType(e_brushType::textureBrush);
    m_painterManager.ImageBrushSelect(e_imageBrush::texture1);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::SetBrushTexture2()
{
    m_painterManager.BrushType(e_brushType::textureBrush);
    m_painterManager.ImageBrushSelect(e_imageBrush::texture2);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::SetBrushCustom()
{
    m_painterManager.BrushType(e_brushType::textureBrush);
    m_painterManager.ImageBrushSelect(e_imageBrush::custom);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::setImageSizeAndClear(const QSize &size)
{
    resizeImage(&m_image, size);
    resizeImage(&m_imageDraw, size);
    //resizeImage(&m_imageEmpty, size);
    QColor Fill = QColor(255, 255, 255, 255);
    m_image.fill(Fill);
    m_imageDraw.fill(QColor(255, 255, 255, 255));
    //m_imageDraw.fill(QColor(255, 255, 255, 0));
    //m_imageEmpty.fill(QColor(255, 255, 255, 0));
    m_floatTexture.Size(size);

    undo_redo_system::setS(UndoRedoSpecialSetIniArgs(m_imageRender->size()));
    undo_redo_system::setSBegin(UndoRedoSpecialBeginArgs(&Fill));
    update();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::ClearImage()
{
    QColor Fill = QColor(255, 255, 255, 255);
    m_image.fill(Fill);
    m_imageDraw.fill(Fill);
    //m_imageDraw.fill(QColor(255, 255, 255, 0));
   // m_imageEmpty.fill(QColor(255, 255, 255, 0));
    m_floatTexture.Clear();
    m_modified = true;

    undo_redo_system::setS(UndoRedoSpecialSetIniArgs(m_imageRender->size()));
    undo_redo_system::setSBegin(UndoRedoSpecialBeginArgs(&Fill));

    update();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::ClearColor()
{
    QColor Fill = QColor(255, 255, 255, 0);
    m_image.fill(Fill);
    m_imageDraw.fill(Fill);
    m_imageDraw.fill(Fill);
    m_floatTexture.Clear(true);
    m_modified = true;

    undo_redo_system::setS(UndoRedoSpecialSetIniArgs(m_imageRender->size()));
    undo_redo_system::setSBegin(UndoRedoSpecialBeginArgs(&Fill));
    update();
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        // zmenit na pos()
        //m_lastPoint = event->position().toPoint();
        m_lastPoint = event->pos();
        m_measurePoint = event->pos();
        m_firstHit = true;
        qInfo() << "- mousePressEvent()";
        //debug_paint_brush(m_painterManager.GetBrush());
        undo_redo_system::setSBegin(UndoRedoSpecialBeginArgs(m_painterManager.GetBrush(), &m_lastPoint));
        drawLineTo(event->pos());
        m_painting = true;
        ghf::areaSet(m_drawedArea, m_storedArea);
    }
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_painting)
    {
        //m_imageRender = &m_imageDraw;
        drawLineTo(event->pos());
    }
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_painting)
    {
        ghf::areaMax(m_drawedArea, m_image.size());
        m_painting = false;
        drawBoundingBox(m_drawedArea);
        undo_redo_system::setSEnd(UndoRedoSpecialEndArgs(m_drawedArea, m_imageRender));
        /*UndoRedoArgs ura(m_drawedArea, &m_image, &m_floatTexture);
        undo_redo_system::set(ura);*/
        ghf::areaReset(m_drawedArea);

    }
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dRect = event->rect();
    //painter.drawImage(dRect, m_image, dRect);
    //painter.drawImage(dRect, m_imageDraw, dRect); //m_imageRender
    painter.drawImage(dRect, *m_imageRender, dRect);
}

// ----------------------------------------------------------------------------------------------------

/*
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
*/

// ----------------------------------------------------------------------------------------------------

void Little_Painter::drawLineTo(const QPoint &endPoint)
{
    //QPainter painter(&m_imageDraw);// m_imageRender
    //QPainter painter(&m_image);
    QPainter painter(m_imageRender);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QRect area;

    m_painterManager.draw(DrawManagerArgs(&painter, &endPoint, &area, &m_lastPoint, &m_measurePoint)); // painter, endPoint, area

    m_storedArea = ghf::AreaPixels(area.top(), area.left(), area.width(), area.height());
    m_storedArea.LeftA = area.left();
    m_storedArea.TopA = area.top();
    m_storedArea.RightA = area.right();
    m_storedArea.BottomA = area.bottom();

    ghf::areaAddA(m_drawedArea, m_storedArea);
    /*if (m_floatTexture.SetImage(m_image, area) == false)
    {
        QMessageBox::warning(this, "Warning", "Image was not copied");
    }*/
    update(area);
    undo_redo_system::setSActive(UndoRedoSpecialActiveArgs(&m_lastPoint));
    m_lastPoint = endPoint;
}

void Little_Painter::drawBoundingBox()
{
    QPainter painter(m_imageRender);
    painter.setPen(QColor(255, 128, 0, 255));
    ghf::AreaPixels * ap = &m_drawedArea; // m_drawedArea
    QRect a = QRect(ap->Left, ap->Top, ap->Right, ap->Bottom);
    painter.drawRect(QRect(ap->Left , ap->Top , ap->Right -1, ap->Bottom -1));
    update(a);
}

void Little_Painter::drawBoundingBox(const graphic_helper_functions::AreaPixels area)
{
    QPainter painter(m_imageRender);
    painter.setPen(QColor(255, 128, 0, 255));
    QRect a = QRect(area.Left , area.Top , area.Right, area.Bottom);
    painter.drawRect(QRect(area.Left , area.Top , area.Right -1, area.Bottom -1));
    update(a);
}

// ----------------------------------------------------------------------------------------------------

void Little_Painter::resizeImage(QImage *image, const QSize &newSize)
{
    QImage nImage(newSize, QImage::Format_RGBA8888_Premultiplied);
    *image = nImage;
}
