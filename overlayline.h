#ifndef OVERLAYLINE_H
#define OVERLAYLINE_H


#include "overlaywidget.h"


class QPainter;


class OverlayLine : public OverlayWidget
{
    Q_OBJECT

    QColor m_color;
    unsigned m_thickness;


public:
    OverlayLine(QWidget *parent = nullptr);


    void setThickness(unsigned thickness) {
        m_thickness = thickness;
    }


    void setColor(QColor color) {
        m_color = color;
    }


protected:
    void paintEvent(QPaintEvent *);
};


#endif // OVERLAYLINE_H
