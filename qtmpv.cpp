#include <clocale>
#include <sstream>
#include <stdexcept>

#include <QtGlobal>
#include <QEvent>
#include <QStatusBar>
#include <QTextEdit>
#include <QLabel>
#include <QTimer>

#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#endif

#include "qtmpv.h"

#include "qthelper.hpp"
#include "overlayline.h"
#include "overlaytext.h"

static void wakeup(void *ctx)
{
    MainWindow *mainwindow = (MainWindow *)ctx;
    emit mainwindow->mpv_events();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt mpv");
    setMinimumSize(800, 450);

    statusBar();


    QMainWindow *log_window = new QMainWindow(this);
    log = new QTextEdit(log_window);
    log->setReadOnly(true);
    log_window->setCentralWidget(log);
    log_window->setWindowTitle("qtmpv log window");
    log_window->setMinimumSize(500, 50);
    log_window->show();


    mpv = mpv_create();
    if (!mpv)
        throw std::runtime_error("can't create mpv instance");

    mpv_container = new QWidget(this);
    setCentralWidget(mpv_container);
    mpv_container->setAttribute(Qt::WA_DontCreateNativeAncestors);
    mpv_container->setAttribute(Qt::WA_NativeWindow);
    auto raw_wid = mpv_container->winId();


#ifdef _WIN32
#else
    int64_t wid = raw_wid;
#endif

    mpv_set_option(mpv, "wid", MPV_FORMAT_INT64, &wid);
    mpv_set_option_string(mpv, "input-default-bindings", "yes");
    mpv_set_option_string(mpv, "input-vo-keyboard", "yes");
    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "track-list", MPV_FORMAT_NODE);
    mpv_observe_property(mpv, 0, "chapter-list", MPV_FORMAT_NODE);
    mpv_request_log_messages(mpv, "info");

    connect(this, &MainWindow::mpv_events, this,
            &MainWindow::on_mpv_events, Qt::QueuedConnection);

    mpv_set_wakeup_callback(mpv, wakeup, this);


    if (mpv_initialize(mpv) < 0)
        throw std::runtime_error("mpv failed to initialize");

    open_video_file("/dev/video0");

    overlayLine = new OverlayLine(mpv_container);
    overlayLine->setColor(Qt::red);
    overlayLine->setThickness(3);

    overlayText = new OverlayText(mpv_container);
    overlayText->setText("Hello, World!!!");
    overlayText->setFont(QFont());
    overlayText->setFontSize(100);
    overlayText->setFontColor(Qt::gray);
}


bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Show:
        overlayLine->show();
        overlayText->show();
        break;

    case QEvent::WindowActivate:
    case QEvent::Resize:
    case QEvent::Move:
        overlayLine->widgetSizeMove(mpv_container);
        overlayText->widgetSizeMove(mpv_container);
        break;
    default:
        break;
    }

    return QMainWindow::event(event);
}


void MainWindow::handle_mpv_event(mpv_event *event)
{
    switch (event->event_id) {
    case MPV_EVENT_PROPERTY_CHANGE: {
        mpv_event_property *prop = (mpv_event_property *)event->data;
        if (strcmp(prop->name, "time-pos") == 0) {
            if (prop->format == MPV_FORMAT_DOUBLE) {
                double time = *(double *)prop->data;
                std::stringstream ss;
                ss << "At: " << time;
                statusBar()->showMessage(QString::fromStdString(ss.str()));
            } else if (prop->format == MPV_FORMAT_NONE) {
                statusBar()->showMessage("");
            }
        } else if (strcmp(prop->name, "chapter-list") == 0 ||
                   strcmp(prop->name, "track-list") == 0)
        {
#if QT_VERSION >= 0x050000
            if (prop->format == MPV_FORMAT_NODE) {
                QVariant v = mpv::qt::node_to_variant((mpv_node *)prop->data);
                QJsonDocument d = QJsonDocument::fromVariant(v);
                append_log("Change property " + QString(prop->name) + ":\n");
                append_log(d.toJson().data());
            }
#endif
        }
        break;
    }
    case MPV_EVENT_VIDEO_RECONFIG: {
        int64_t w, h;
        if (mpv_get_property(mpv, "dwidth", MPV_FORMAT_INT64, &w) >= 0 &&
            mpv_get_property(mpv, "dheight", MPV_FORMAT_INT64, &h) >= 0 &&
            w > 0 && h > 0)
        {
            std::stringstream ss;
            ss << "Reconfig: " << w << " " << h;
            statusBar()->showMessage(QString::fromStdString(ss.str()));
        }
        break;
    }
    case MPV_EVENT_LOG_MESSAGE: {
        struct mpv_event_log_message *msg = (struct mpv_event_log_message *)event->data;
        std::stringstream ss;
        ss << "[" << msg->prefix << "] " << msg->level << ": " << msg->text;
        append_log(QString::fromStdString(ss.str()));
        break;
    }
    case MPV_EVENT_SHUTDOWN: {
        mpv_terminate_destroy(mpv);
        mpv = NULL;
        break;
    }
    default: ;
    }
}


void MainWindow::on_mpv_events()
{
    while (mpv) {
        mpv_event *event = mpv_wait_event(mpv, 0);
        if (event->event_id == MPV_EVENT_NONE)
            break;
        handle_mpv_event(event);
    }
}


void MainWindow::open_video_file(const QString &filename)
{
    if (mpv) {
        const QByteArray c_filename = filename.toUtf8();
        const char *args[] = {"loadfile", c_filename.data(), NULL};
        mpv_command_async(mpv, 0, args);
    }
}


void MainWindow::append_log(const QString &text)
{
    QTextCursor cursor = log->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(text);
    log->setTextCursor(cursor);
}


MainWindow::~MainWindow()
{
    if (mpv)
        mpv_terminate_destroy(mpv);
}


void MainWindow::resizeEvent(QResizeEvent *)
{
    overlayLine->setOriginOffset(0, this->height()/2);
    overlayLine->resize(this->width(), overlayLine->height());

    overlayText->setOriginOffset(0, 0);
    overlayText->resize(100, 100);
}
