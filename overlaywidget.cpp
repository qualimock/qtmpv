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


void OverlayWidget::widgetSizeMove(QWidget *origin)
{
    if (this->width() <= origin->width() && this->height() <= origin->height()) {
        this->setWindowOpacity(1);

        QPoint originPos = origin->mapToGlobal(origin->pos());

        this->move(originPos.x() + originOffset.x(), originPos.y() + originOffset.y());

        this->raise();
    } else {
        this->setWindowOpacity(0);
    }
}
