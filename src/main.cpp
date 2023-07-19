#include <QApplication>

#include "qtmpv.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::setlocale(LC_NUMERIC, "C");

    MainWindow w;
    w.show();

    return a.exec();
}
