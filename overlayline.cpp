#include "overlayline.h"

#include <QPainter>

OverlayLine::OverlayLine(QWidget *parent)
    : OverlayWidget(parent)
{}


void OverlayLine::setThickness(unsigned thickness)
{
    m_thickness = thickness;
}


void OverlayLine::setColor(QColor color)
{
    m_color = color;
}


void OverlayLine::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(m_color, m_thickness));
    painter.drawLine(0, this->height()/2, this->width(), this->height()/2);
}
