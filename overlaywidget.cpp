#include "overlaywidget.h"

#include <QPainter>

OverlayWidget::OverlayWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void OverlayWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 3));
    painter.drawLine(0, this->height()/2, this->width(), this->height()/2);
}


void OverlayWidget::widgetSizeMove(QWidget *origin)
{
    if (this->width() <= origin->width() && this->height() <= origin->height()) {
        this->setWindowOpacity(1);

        QPoint p = origin->mapToGlobal(origin->pos());

        this->move(p.x(), p.y()+origin->height()/2);
        this->resize(origin->width(), this->height());
        this->raise();
    } else {
        this->setWindowOpacity(0);
    }
}
