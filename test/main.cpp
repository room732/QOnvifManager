#include "mainwindow.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qWarning() << "start QOnvifManager";
    MainWindow w;
    w.show();

    return a.exec();
}
