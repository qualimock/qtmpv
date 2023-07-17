#ifndef QTEXAMPLE_H
#define QTEXAMPLE_H

#include <QMainWindow>

#include <mpv/client.h>

class QTextEdit;
class QLabel;
class QPainter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_file_open();
    void on_new_window();
    void on_mpv_events();

signals:
    void mpv_events();

private:
    QWidget *mpv_container;
    QTextEdit *log;
    QLabel *label;
    QPainter *painter;
    mpv_handle *mpv;

    void append_log(const QString &text);

    void create_player();
    void handle_mpv_event(mpv_event *event);
    void paintEvent(QPaintEvent *event);
};

#endif // QTEXAMPLE_H
