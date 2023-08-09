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
    overlayLine->setThickness(3);

    overlayText = new OverlayText(this);
    overlayText->setFont(QFont());
    overlayText->setFontSize(100);

    overlayWidgets.push_back(overlayLine);
    overlayWidgets.push_back(overlayText);
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

    return -1;
}


void MainWindow::update_window_data_loop()
{
    //////////////////////////// MSGGET ////////////////////////////

    sleep(6);

    Msg msg;
    int msgid;
    int key = ftok(FTOK_PATH, 1);
    unsigned uMsg;

    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    if ((msgid = msgget(key, 0666 | IPC_CREAT)) == -1)
    {
        perror("msgget");
        exit(1);
    }

    msg.mType = 0;

    int x = 0, y = 0;

    while (true)
    {
        if (Display* display = XOpenDisplay( NULL ))
        {
            ulong count = 0;
            Window* wins = find_windows(display, &count);
            for (ulong i = 0; i < count; i++)
            {
                Window w = wins[ i ];

                int windowPID = get_window_pid(display, w);

                if (windowPID == -1)
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
        }

        if (x != attrs.x || y != attrs.y)
        {
            x = attrs.x, y = attrs.y;

            overlayLine->move(x, y + attrs.height / 2);
            overlayLine->resize(attrs.width, 3);
            overlayLine->update();

            overlayText->move(x, y);
        }

        //////////////////////////// MSGGET ////////////////////////////

        msgrcv(msgid, &msg, MSGLEN, 1, 0);

         uMsg = strtoul(msg.mText, nullptr, 10);

        if (uMsg < 10)
            overlayText->setFontColor(Qt::red);
        else if (uMsg < 50)
            overlayText->setFontColor(Qt::yellow);
        else
            overlayText->setFontColor(Qt::green);

        overlayText->setText(msg.mText);
        overlayText->update();
    }
}


bool MainWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::Show:
        for (auto widget : overlayWidgets)
        {
            widget->show();
        }
        break;

    case QEvent::WindowActivate:
    case QEvent::Resize:
    case QEvent::Move:
    default:
        break;
    }

    return QMainWindow::event(event);
}


MainWindow::~MainWindow()
{}


void MainWindow::resizeEvent(QResizeEvent *)
{
    overlayLine->setOriginOffset(0, attrs.y/2);
    overlayLine->resize(attrs.x, attrs.y);

    overlayText->resize(100, 100);
}
