#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <qdebug.h>
#include <QAxObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
