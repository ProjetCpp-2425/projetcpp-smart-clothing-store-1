#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>
#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "arduino.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    Connexion c;
    bool test=c.createconnect();
    Arduino arduino;

        // Essayer de se connecter au port série
        if (arduino.openConnection("COM4")) {  // Remplacez "COMx" par le port série correct (par exemple, "COM3")
            qDebug() << "Connexion série réussie!";
        } else {
            qDebug() << "Échec de la connexion série!";
        }

        // Vérifier si la connexion à la base de données est réussie
        if (test) {
            w.show();
            QMessageBox::information(nullptr, QObject::tr("Database is open"),
                        QObject::tr("Connection successful.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                        QObject::tr("Connection failed.\n"
                                    "Click Cancel to exit."), QMessageBox::Cancel);
        }
    return a.exec();
}
