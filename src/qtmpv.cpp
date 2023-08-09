#include <clocale>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <sys/msg.h>

#include <QtGlobal>
#include <QEvent>
#include <QStatusBar>
#include <QTextEdit>
#include <QLabel>
#include <QDebug>
#include <iostream>
#include <unistd.h>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#endif

#include "qtmpv.hpp"

#include "overlay/overlayline.hpp"
#include "overlay/overlaytext.hpp"


#define ERROR -1
#define FONT_SIZE 100
#define LINE_THICKNESS 3


MainWindow::MainWindow(const pid_t &pid, QWidget *parent)
    : QMainWindow(parent)
{
    if (display == NULL)
    {
        throw std::system_error();
    }

    pidFfplay = pid;

    overlayLine = new OverlayLine(this);
    overlayLine->setColor(Qt::red);
    overlayLine->setThickness(LINE_THICKNESS);

    overlayText = new OverlayText(this);
    overlayText->setFont(QFont());
    overlayText->setFontSize(FONT_SIZE);

    overlayWidgets.push_back(overlayLine);
    overlayWidgets.push_back(overlayText);

    overlayText->show();
    overlayLine->show();
}


Window* MainWindow::find_windows(Display *display, ulong *winCount)
{
    Atom actualType;
    int format;
    ulong bytesAfter;
    uchar* list = NULL;
    Status status = XGetWindowProperty(
        display,
        DefaultRootWindow(display),
        XInternAtom(display, "_NET_CLIENT_LIST", False),
        0L,
        ~0L,
        False,
        XA_WINDOW,
        &actualType,
        &format,
        winCount,
        &bytesAfter,
        &list
        );

    if (status != Success)
    {
        *winCount = 0;
        return NULL;
    }

    return reinterpret_cast<Window *>(list);
}


pid_t MainWindow::get_window_pid(Display *display, Window window)
{
    Atom atomPID = XInternAtom(display, "_NET_WM_PID", True);
    Atom type;
    int format;
    unsigned long nItems;
    unsigned long bytesAfter;
    unsigned char* propPID = nullptr;

    if (XGetWindowProperty(display, window, atomPID, 0, 1, False, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID) == Success)
    {
        if (propPID != nullptr)
        {
            unsigned long pid = *((unsigned long*)propPID);
            XFree(propPID);

            return static_cast<int>(pid);
        }
    }

    return ERROR;
}


void MainWindow::msgget_loop()
{
    Msg msg;
    int msgid;
    int key = ftok(FTOK_PATH, 1);
    unsigned uMsg;

    if (key == ERROR)
    {
        perror("ftok");
        exit(1);
    }

    if ((msgid = msgget(key, 0666 | IPC_CREAT)) == ERROR)
    {
        perror("msgget");
        exit(1);
    }

    msg.mType = 0;

    while (true)
    {
        msgrcv(msgid, &msg, MSGLEN, 1, 0);

        uMsg = strtoul(msg.mText, nullptr, 10);

        if (uMsg < 10)
        {

            overlayText->setFontColor(Qt::red);
        }
        else if (uMsg < 50)
        {
            overlayText->setFontColor(Qt::yellow);
        }
        else
        {
            overlayText->setFontColor(Qt::green);
        }

        overlayText->setText(msg.mText);
        overlayText->update();
    }
}


void MainWindow::x11_loop()
{
    int x = 0, y = 0;
    int hW = 0, wW = 0;
    ulong count;
    Window *wins;
    Window w;
    pid_t windowPID;
    Display *display;

    while (true)
    {
        display = XOpenDisplay(NULL);
        count = 0;
        wins = find_windows(display, &count);

        for (ulong i = 0; i < count; i++)
        {
            w = wins[i];

            windowPID = get_window_pid(display, w);

            if (windowPID == ERROR)
            {
                throw std::system_error();
            }

            if (windowPID == pidFfplay)
            {
                if (XGetWindowAttributes(display, w, &attrs))
                {
                    Window child;
                    if (!XTranslateCoordinates(display, w, attrs.root, 0, 0, &attrs.x, &attrs.y, &child))
                    {
                        throw std::system_error();
                    }
                }

                break;
            }
        }

        if (wins)
        {
            XFree(wins);
        }

        XCloseDisplay(display);

        if (x != attrs.x || y != attrs.y || hW != attrs.height || wW != attrs.width)
        {
            x = attrs.x, y = attrs.y;
            hW = attrs.height, wW = attrs.width;

            delete overlayLine;
            delete overlayText;

            overlayLine = new OverlayLine(this);
            overlayText = new OverlayText(this);

            overlayLine->setGeometry(x, y + hW / 2, wW, 3);
            overlayLine->setColor(Qt::red);

            overlayText->move(x, y);
            overlayText->setFont(QFont());
            overlayText->setFontSize(FONT_SIZE);

            overlayLine->show();
            overlayText->show();
        }

        sleep(1);
    }
}


MainWindow::~MainWindow()
{}
