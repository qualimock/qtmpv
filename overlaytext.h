#ifndef OVERLAYTEXT_H
#define OVERLAYTEXT_H

#include "overlaywidget.h"

class QPainter;
class QString;
class QFont;

class OverlayText : public OverlayWidget
{
    Q_OBJECT

    QColor m_fontColor;
    QFont m_font;
    QString m_text;

public:
    OverlayText(QWidget *parent = nullptr);
    void setText(QString text);
    void setFont(QFont font);
    void setFontSize(unsigned size);
    void setFontColor(QColor color);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // OVERLAYTEXT_H
