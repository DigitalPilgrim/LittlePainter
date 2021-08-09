#ifndef LITTLE_PAINTER_H
#define LITTLE_PAINTER_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include "floattexture.h"

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
    QColor penColor() const { return m_penColor; }
    int penWidth() const { return m_penWidth; }

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
    void drawEnd();
    void resizeImage(QImage* image, const QSize &newSize);

    bool m_modified = false;
    bool m_painting = false;
    int m_penWidth = 1;
    QColor m_penColor = Qt::blue;
    QImage m_image;
    QPoint m_lastPoint;

    FloatTexture m_floatTexture;

};


#endif // LITTLE_PAINTER_H
