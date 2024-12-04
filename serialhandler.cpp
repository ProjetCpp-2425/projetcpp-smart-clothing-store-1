#include "serialhandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SerialHandler::SerialHandler(QObject *parent) : QObject(parent) {
    connect(&serial, &QSerialPort::readyRead, this, &SerialHandler::handleReadyRead);
}

bool SerialHandler::open(const QString &portName) {
    // Configure the serial port
    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully on:" << portName;
        return true;
    } else {
        qDebug() << "Failed to open serial port:" << serial.errorString();
        return false;
    }
}

void SerialHandler::handleReadyRead() {
    QByteArray data = serial.readAll();
    QString userID = QString(data).trimmed(); // Trim whitespace

    qDebug() << "Received ID:" << userID;

    // Validate the user ID
    if (isValidUser(userID)) {
        serial.write("VALID\n");
        qDebug() << "ID is valid. Response sent.";
    } else {
        serial.write("INVALID\n");
        qDebug() << "ID is invalid. Response sent.";
    }
}

bool SerialHandler::isValidUser(const QString &userID) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM EMPLOYES WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", userID);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    } else {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }
}
