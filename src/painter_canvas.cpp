#include "painter_canvas.h"


painter_canvas::painter_canvas(QWidget *parent) : QWidget(parent)
{

}

painter_canvas::painter_canvas(QWidget *parent, QWidget *painter_area)
     : QWidget(parent)
     , m_painter_area(painter_area)
{

}

void painter_canvas::mousePressEvent(QMouseEvent *event)
{

}

void painter_canvas::mouseMoveEvent(QMouseEvent *event)
{
    draw(event->pos());
}

void painter_canvas::mouseReleaseEvent(QMouseEvent *event)
{

}

void painter_canvas::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBackground(QBrush(QColor(0, 0, 0, 0)));
    QPen pen = p.pen();
    pen.setStyle(Qt::PenStyle::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    p.setPen(pen);
    //p.setBrush(Qt::blue);
    int x = m_mousePoint.x()
            , y = m_mousePoint.y()
            , r = 50;
    p.drawEllipse(x, y, r, r);
}

void painter_canvas::resizeEvent(QResizeEvent *event)
{

}

void painter_canvas::draw(const QPoint &mousePoint)
{
    m_mousePoint = mousePoint;
    update();
}
