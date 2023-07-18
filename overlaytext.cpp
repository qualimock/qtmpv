#include "overlaytext.h"

#include <QPainter>
#include <QString>
#include <QFont>

OverlayText::OverlayText(QWidget *parent)
    : OverlayWidget(parent)
{}

void OverlayText::setText(QString text)
{
    m_text = text;
}

void OverlayText::setFontColor(QColor color)
{
    m_fontColor = color;
}

void OverlayText::setFont(QFont font)
{
    m_font = font;
}

void OverlayText::setFontSize(unsigned size)
{
    m_font.setPixelSize(size);
}

void OverlayText::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(m_fontColor));
    resize(QFontMetrics(m_font).size(0, m_text));
    painter.setFont(m_font);
    painter.drawText(rect(), Qt::AlignJustify, m_text);
}
