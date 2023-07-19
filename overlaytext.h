#ifndef OVERLAYTEXT_H
#define OVERLAYTEXT_H


#include "overlaywidget.h"


class QPainter;
class QString;
class QFont;


class OverlayText : public OverlayWidget
{
    Q_OBJECT

    QString m_text;
    QFont m_font;
    QColor m_fontColor;


public:
    OverlayText(QWidget *parent = nullptr);


    void setText(QString text) {
        m_text = text;
    }


    void setFontColor(QColor color) {
        m_fontColor = color;
    }


    void setFont(QFont font) {
        m_font = font;
    }


    void setFontSize(unsigned size) {
        m_font.setPixelSize(size);
    }


protected:
    void paintEvent(QPaintEvent *);
};

#endif // OVERLAYTEXT_H
