#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool openConnection(const QString &portName, int baudRate = 9600);
    void closeConnection();
    bool isConnected() const;
    //void writeData(const QString &data);
   // void writeDataToArduino(const QByteArray &data);
    void writeData(const QByteArray &data);



signals:
    void clientFound(const QString &uid);  // Signal pour envoyer l'UID formaté



private slots:
    void readData();  // Slot pour lire les données du port série

private:
    QSerialPort *serialPort;
    bool connected;
};

#endif // ARDUINO_H
