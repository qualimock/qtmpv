#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QWidget>
#include <QPoint>

class OverlayWidget : public QWidget
{
    Q_OBJECT

    QPoint originOffset = QPoint(0, 0);

public:
    explicit OverlayWidget(QWidget *parent = nullptr);

    void setOriginOffset(const QPoint &offset) {
        originOffset = offset;
    }
    void setOriginOffset(int x, int y) {
        originOffset = QPoint(x, y);
    }

public slots:
    void widgetSizeMove(QWidget *origin);
};

#endif // OVERLAYWIDGET_H
