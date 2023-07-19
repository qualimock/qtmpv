#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H


#include <QWidget>


class QPoint;


class OverlayWidget : public QWidget
{
    Q_OBJECT

    QPoint m_originOffset;


public:
    explicit OverlayWidget(QWidget *parent = nullptr);

    void setOriginOffset(const QPoint &offset) {
        m_originOffset = offset;
    }
    void setOriginOffset(int x, int y) {
        m_originOffset = QPoint(x, y);
    }


public slots:
    void widgetResizeMove(QWidget *origin);
};


#endif // OVERLAYWIDGET_H
