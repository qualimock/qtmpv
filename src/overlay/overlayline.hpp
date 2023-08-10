#ifndef OVERLAYLINE_HPP
#define OVERLAYLINE_HPP


#include "overlaywidget.hpp"


class QPainter;


class OverlayLine : public OverlayWidget
{
    Q_OBJECT

    QColor m_color;
    unsigned m_thickness;


public:
    OverlayLine(QWidget *parent = nullptr);
    OverlayLine(OverlayWidget&& widget);

    OverlayLine(const QColor& color, unsigned thickness, QWidget *parent = nullptr);


    void setThickness(unsigned thickness) {
        m_thickness = thickness;
    }


    void setColor(QColor color) {
        m_color = color;
    }


protected:
    void paintEvent(QPaintEvent *);
};


#endif // OVERLAYLINE_HPP
