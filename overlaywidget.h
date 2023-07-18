#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>

class OverlayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OverlayWidget(QWidget *parent = nullptr);

public slots:
    void widgetSizeMove(QWidget *origin);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // OVERLAYWIDGET_H
