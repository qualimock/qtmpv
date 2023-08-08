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

    //////////////////////////// X11 ////////////////////////////

    ulong count = 0;
    Window *wins = find_windows(display, &count);

    for (ulong i = 0; i < count; i++)
    {
        Window w = wins[i];

        pid_t windowPID = get_window_pid(display, w);

        if (windowPID == -1)
        {
            throw std::system_error();
        }

        if (windowPID == pidFfplay)
        {
            *ffplayWindow = w;
        }
    }

    while (true)
    {
        /////////////////////////// X11 ////////////////////////////

        if (XGetWindowAttributes(display, *ffplayWindow, &attrs))
        {
            Window child;
            if (!XTranslateCoordinates(display, *ffplayWindow, attrs.root, 0, 0, &attrs.x, &attrs.y, &child))
            {
                throw std::system_error();
            }
            else
            {
                std::cout << attrs.x << ' ' << attrs.y << std::endl;
            }
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
    static int x = attrs.x, y = attrs.y;

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
        break;
    default:
        break;
    }

    if (x != attrs.x || y != attrs.y)
    {
        std::cout << "Moved!" << std::endl;
        x = attrs.x, y = attrs.y;
        for (auto widget : overlayWidgets)
        {
            widget->move(x, y);
            widget->update();
        }
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
