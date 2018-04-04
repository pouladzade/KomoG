#include "mainwindow.h"
#include <QApplication>
#include"config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!Config::getInstance()->LoadConfig())
    {
        return EXIT_FAILURE;
    }
    MainWindow w;
    w.show();

    return a.exec();
}
