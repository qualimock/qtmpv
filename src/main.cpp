#include <QApplication>
#include <thread>
#include <filesystem>
#include <vector>
#include <iostream>

#include "qtmpv.h"


static std::string ask_about_stream()
{
    std::vector<std::string> allDirs;
    std::vector<std::string> videoFiles;

    long userAnswer;

    if (std::filesystem::is_directory(FTOK_PATH))
    {
        for (const auto &entry : std::filesystem::directory_iterator(FTOK_PATH))
        {
            allDirs.push_back(entry.path().filename().string());
        }
    }

    for (const auto &file : allDirs)
    {
        if (file.find("video") == std::string::npos)
        {
            videoFiles.push_back(file);
        }
    }

    if (videoFiles.size() == 1)
    {
        return videoFiles.at(0);
    }

    std::cout << "Какой поток для вывода вы хотите использовать?:" << std::endl;

    for (unsigned i = 0; i < videoFiles.size(); i++)
    {
        std::cout << i + 1 << ". " << videoFiles.at(i) << std::endl;
    }

    std::cout << "Введите только число >> ";
    std::cin >> userAnswer;

    try
    {
        return videoFiles.at(userAnswer - 1);
    }
    catch (...)
    {
        perror("Плохой индекс");
        exit(1);
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::setlocale(LC_NUMERIC, "C");

    std::string stream = ask_about_stream();

    MainWindow w;

    w.set_path_of_video_stream(stream);

    std::thread text_thread(&MainWindow::update_text_loop, &w);
    text_thread.detach();

    w.show();

    return a.exec();
}
