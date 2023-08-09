#include <QApplication>
#include <thread>
#include <filesystem>
#include <vector>
#include <iostream>
#include <QProcess>

#include "qtmpv.hpp"


#define DEV_PATH "/dev/"


static std::string ask_about_stream()
{
    std::vector<std::string> allDirs;
    std::vector<std::string> videoFiles;

    long userAnswer;

    if (std::filesystem::is_directory(DEV_PATH))
    {
        for (const auto &entry : std::filesystem::directory_iterator(DEV_PATH))
        {
            allDirs.push_back(entry.path().filename().string());
        }
    }

    for (const auto &file : allDirs)
    {
        if (file.find("video") != std::string::npos)
        {
            videoFiles.push_back(file);
        }
    }

    if (videoFiles.size() == 1)
    {
        return DEV_PATH + videoFiles.at(0);
    }

    std::cout << "Какой поток для вывода вы хотите использовать? (Обычно это /dev/video0):" << std::endl;

    for (unsigned i = 0; i < videoFiles.size(); i++)
    {
        std::cout << i + 1 << ". " << DEV_PATH << videoFiles.at(i) << std::endl;
    }

    std::cout << "Введите только число >> ";
    std::cin >> userAnswer;

    try
    {
        return DEV_PATH + videoFiles.at(userAnswer - 1);
    }
    catch (...)
    {
        throw std::length_error("Выбран неправильный индекс");
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::setlocale(LC_NUMERIC, "C");

    std::string stream = ask_about_stream();

    QProcess process;
    QStringList args = {"-video_size", "1920x1080", "-framerate", "30", stream.c_str()};
    process.start("ffplay", args);

    MainWindow w(process.processId());

    std::thread textThread(&MainWindow::msgget_loop, &w);
    std::thread x11Thread(&MainWindow::x11_loop, &w);

    textThread.detach();
    x11Thread.detach();

    w.show();

    return a.exec();
}
