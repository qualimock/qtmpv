#ifndef QTEXAMPLE_H
#define QTEXAMPLE_H

#include <QMainWindow>
#include <mpv/client.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>


class QTextEdit;
class OverlayWidget;
class OverlayLine;
class OverlayText;


#define MSGLEN 32
#define FTOK_PATH "/etc/qtmpv/token.txt"


struct Msg
{
    long mType;
    char mText[MSGLEN];
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVector<OverlayWidget *> overlayWidgets;
    OverlayLine *overlayLine;
    OverlayText *overlayText;

    Window *ffplayWindow;
    Display *display = XOpenDisplay(NULL);
    XWindowAttributes attrs;
    pid_t pidFfplay;

    Window* find_windows(Display *display, ulong *winCount);
    pid_t get_window_pid(Display *display, Window window);

public:
    explicit MainWindow(const pid_t &pid, QWidget *parent = 0);
    ~MainWindow();
    void update_window_data_loop();

protected:
    void resizeEvent(QResizeEvent *event);
    bool event(QEvent *event);

};


#endif // QTEXAMPLE_H
