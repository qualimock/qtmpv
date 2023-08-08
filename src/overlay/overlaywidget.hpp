#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H


#include <QWidget>


class QPoint;


class OverlayWidget : public QWidget
{
    Q_OBJECT

    QPoint m_originOffset;


public:
    /** Widget that lays over the parent */
    OverlayWidget(QWidget *parent = nullptr);


    /** Widget constructor with offset from the upper left corner of parent */
    OverlayWidget(const QPoint& offset, QWidget *parent = nullptr);

    /** Widget constructor with offset from the upper left corner of parent */
    OverlayWidget(int offset_x, int offset_y, QWidget *parent = nullptr);


    OverlayWidget(OverlayWidget&& wgt);
    OverlayWidget(const OverlayWidget& wgt);


    /** Sets the offset from the upper left corner of parent */
    void setOriginOffset(const QPoint& offset) {
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
