#ifndef LITTLE_PAINTER_H
#define LITTLE_PAINTER_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "floattexture.h"
#include "image_manager.h"
#include "painter_manager.h"

class Little_Painter : public QWidget
{
    Q_OBJECT
public:
    explicit Little_Painter(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char* fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return m_modified; }
    QColor penColor() const;
    int penWidth() const;
    void SetBrushType(int value);
    void SetBrushCircle();
    void SetBrushTexture();

public slots:
    void ClearImage();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:

private:
    void drawLineTo(const QPoint &endPoint);
    void drawLineToBrush(int &pointsX, int &pointsY, int &posX, int &posY, int& radius, QPainter& painter);
    void resizeImage(QImage* image, const QSize &newSize);

    bool m_modified = false;
    bool m_painting = false;
    bool m_firstHit = false;
    QImage m_image;
    QPoint m_lastPoint;
    QPoint m_measurePoint;

    FloatTexture m_floatTexture;

    painter_manager m_painterManager;

    image_manager m_im;

};


#endif // LITTLE_PAINTER_H
