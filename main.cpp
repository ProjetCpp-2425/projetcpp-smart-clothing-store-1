#include "mainwindow.h"
#include "QApplication"
#include "QPushButton"
#include "QIcon"
#include "connection.h" // Include the connection header
#include <QMessageBox>
#include <cstddef>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    Connection c;
    bool test=c.createconnection();



    MainWindow w;
    if(test){
        w.show();
        QMessageBox::information(nullptr,QObject::tr("database is open"),
                                 QObject::tr("connection successful.\n""click cancel to erxist."),QMessageBox::Cancel);


    }
    else
        QMessageBox::information(nullptr,QObject::tr("database is not open"),
                                 QObject::tr("connection mouch successful.\n""click cancel to erxist."),QMessageBox::Cancel);




    return a.exec();
}

