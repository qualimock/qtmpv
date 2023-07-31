#include "overlayline.h"

#include <QPainter>


OverlayLine::OverlayLine(QWidget *parent)
    : OverlayWidget(parent)
    , m_color(Qt::black)
    , m_thickness(10)
{}


OverlayLine::OverlayLine(OverlayWidget&& widget)
    : OverlayWidget(widget)
    , m_color(Qt::black)
    , m_thickness(10)
{}


OverlayLine::OverlayLine(const QColor& color, unsigned thickness, QWidget *parent)
    : OverlayWidget(parent)
{

}


void OverlayLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(m_color, m_thickness));
    painter.drawLine(0, this->height()/2, this->width(), this->height()/2);
}
