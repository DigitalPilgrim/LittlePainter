#ifndef PAINTER_CANVAS_H
#define PAINTER_CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>

/*  ============================================================================================================
 *  !! TATO TRIEDA SA ZATIAL NEPOUZIVA !!
 *  ============================================================================================================
 * */

class painter_canvas : public QWidget
{
    Q_OBJECT

    QWidget * m_painter_area = nullptr;
    QPoint m_mousePoint;
public:
    explicit painter_canvas(QWidget *parent = nullptr);
    painter_canvas(QWidget *parent, QWidget *painter_area);

signals:

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void draw(const QPoint & mousePoint);
};

#endif // PAINTER_CANVAS_H
