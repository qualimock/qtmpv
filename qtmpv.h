#ifndef QTEXAMPLE_H
#define QTEXAMPLE_H

#include <QMainWindow>

#include <mpv/client.h>


class QTextEdit;
class OverlayLine;
class OverlayText;


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_mpv_events();


signals:
    void mpv_events();


private:
    QWidget *mpv_container;
    OverlayLine *overlayLine;
    OverlayText *overlayText;
    QTextEdit *log;
    mpv_handle *mpv;

    void append_log(const QString &text);
    void handle_mpv_event(mpv_event *event);
    void open_video_file(const QString &filename);

protected:
    bool event(QEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // QTEXAMPLE_H
