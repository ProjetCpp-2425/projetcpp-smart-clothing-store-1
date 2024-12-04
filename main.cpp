#include "mainwindow.h"
#include "connection.h" // Include the connection header
#include "serialhandler.h" // Include the SerialHandler header
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Step 1: Establish database connection
    Connection c;
    bool dbConnected = c.createconnection();

    if (!dbConnected) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Failed to connect to the database.\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1; // Exit if the database connection fails
    }

    // Step 2: Initialize and open serial communication
    SerialHandler serialHandler; // Create the serial handler
    if (!serialHandler.open("COM5")) { // Replace "COM3" with your Arduino's port
        QMessageBox::critical(nullptr, QObject::tr("Serial Port Error"),
                              QObject::tr("Failed to open the serial port.\nClick Cancel to exit."), QMessageBox::Cancel);
        return -1; // Exit if the serial port fails to open
    }

    // Step 3: Launch the main window
    MainWindow w;
    w.show();

    QMessageBox::information(nullptr, QObject::tr("Setup Successful"),
                             QObject::tr("Database and Serial Port are ready.\nClick OK to proceed."));

    return a.exec();
}
