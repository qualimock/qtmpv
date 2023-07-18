#include "overlaywidget.h"

#include <QPainter>

OverlayWidget::OverlayWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_PaintOnScreen);

    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void OverlayWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    QString text = "Some foo goes here";
    QFontMetrics metrics(p.font());
    resize(metrics.size(0, text));
    p.drawText(rect(), Qt::AlignCenter, text);
}
