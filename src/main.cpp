#include <QApplication>
#include <thread>

#include "qtmpv.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::setlocale(LC_NUMERIC, "C");

    MainWindow w;

    std::thread text_thread(&MainWindow::update_text_loop, &w);
    text_thread.detach();

    w.show();

    return a.exec();
}
