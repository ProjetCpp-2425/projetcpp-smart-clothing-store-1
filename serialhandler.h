#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class SerialHandler : public QObject {
    Q_OBJECT

public:
    explicit SerialHandler(QObject *parent = nullptr);
    bool open(const QString &portName);

private slots:
    void handleReadyRead();

private:
    QSerialPort serial;               // Serial port object for communication
    bool isValidUser(const QString &userID); // Checks if the user ID exists in the database
};

#endif // SERIALHANDLER_H
