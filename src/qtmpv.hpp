#ifndef QTEXAMPLE_HPP
#define QTEXAMPLE_HPP

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





class MainWindow
{
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
    explicit MainWindow(const pid_t &pid);
    ~MainWindow();
    void msgget_loop();
    void x11_loop();

};


#endif // QTEXAMPLE_HPP
