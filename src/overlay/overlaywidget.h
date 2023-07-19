#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H


#include <QWidget>


class QPoint;


class OverlayWidget : public QWidget
{
    Q_OBJECT

    QPoint m_originOffset;


public:
    OverlayWidget(QWidget *parent = nullptr);
    OverlayWidget(const QPoint &originOffset, QWidget *parent = nullptr);
    OverlayWidget(int offset_x, int offset_y, QWidget *parent = nullptr);

    /** Sets the offset from the upper left corner of parent */
    void setOriginOffset(const QPoint &offset) {
        m_originOffset = offset;
    }
    /** Sets the offset from the upper left corner of parent */
    void setOriginOffset(int x, int y) {
        m_originOffset = QPoint(x, y);
    }


public slots:
    void widgetResizeMove(QWidget *origin);
};


#endif // OVERLAYWIDGET_H
