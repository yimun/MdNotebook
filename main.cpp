#include <QApplication>
#include "mainwindow.h"
#include "dbconnect.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createDBConnection())
    {
        QMessageBox::information(NULL,zh("数据库错误"), zh("创建数据库错误，点击退出"), QMessageBox::Cancel);
        return 0;
    }
    MainWindow w;
    w.show();
    w.resize(800,500);
    return a.exec();
}
