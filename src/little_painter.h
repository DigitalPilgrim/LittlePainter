#ifndef LITTLE_PAINTER_H
#define LITTLE_PAINTER_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "floattexture.h"
#include "image_manager.h"
#include "painter_manager.h"
#include "graphic_helper_functions.h"
#include <list>

class Little_Painter : public QWidget
{
    Q_OBJECT
public:
    explicit Little_Painter(QWidget *parent = nullptr);

    void Undo();
    void Redo();

    bool openImage(/*const QString &fileName*/);
    bool saveImage(/*const QString &fileName, const char* fileFormat*/);
    bool saveImageAs();
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return m_modified; }
    QColor penColor() const;
    int penWidth() const;

    void BrushTransparency(const float & value);
    float BrushTransparency() const;
    void SetBrushType(int value);
    void SetBrushCircle();
    void SetBrushTexture();
    void SetBrushTexture2();
    void SetBrushCustom();

    void setImageSizeAndClear(const QSize& size);

public slots:
    void ClearImage();
    void ClearColor();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    //void resizeEvent(QResizeEvent *event) override;

signals:

private:
    void drawLineTo(const QPoint &endPoint);
    void drawBoundingBox();
    void drawBoundingBox(const ghf::AreaPixels area);
    void resizeImage(QImage* image, const QSize &newSize);

    bool m_modified = false;
    bool m_painting = false;
    bool m_firstHit = false;
    // oblasti do ktorej sa uklada
    QImage m_image;
    //std::list<QImage> m_images;
    // oblast do ktorej sa maluje
    QImage m_imageDraw;
    QImage m_imageEmpty;
    QImage * m_imageRender = nullptr;

    QPoint m_lastPoint;
    QPoint m_measurePoint;

    FloatTexture m_floatTexture;

    painter_manager m_painterManager;

    image_manager m_im;

    ghf::AreaPixels m_drawedArea;
    ghf::AreaPixels m_storedArea;

};


#endif // LITTLE_PAINTER_H
