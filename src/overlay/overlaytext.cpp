#include "overlaytext.hpp"


#include <QPainter>
#include <QString>
#include <QFont>


OverlayText::OverlayText(QWidget *parent)
    : OverlayWidget(parent)
    , m_text("No messages!")
    , m_font(QFont())
    , m_fontColor(Qt::black)
{}


void OverlayText::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(m_fontColor));
    resize(QFontMetrics(m_font).size(0, m_text));
    painter.setFont(m_font);
    painter.drawText(rect(), Qt::AlignJustify, m_text);
}
