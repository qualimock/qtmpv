#ifndef OVERLAYTEXT_HPP
#define OVERLAYTEXT_HPP


#include "overlaywidget.hpp"


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


    QString text() const {
        return m_text;
    }


    QFont font() const {
        return m_font;
    }

    unsigned fontSize() const {
        return m_font.pixelSize();
    }


protected:
    void paintEvent(QPaintEvent *);
};

#endif // OVERLAYTEXT_HPP
