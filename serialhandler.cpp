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

#include <QMessageBox>

#include <QMessageBox>

void SerialHandler::handleReadyRead() {
    static QString buffer; // Tampon pour accumuler les caractères entrants
    QByteArray data = serial.readAll();
    QString receivedData = QString(data).trimmed(); // Nettoyer les espaces

    qDebug() << "Données reçues :" << receivedData;

    // Ajouter les données reçues au tampon
    buffer += receivedData;

    // Vérifier si le tampon contient exactement 4 caractères
    if (buffer.length() == 4) {
        QString userID = buffer; // Extraire l'ID complet
        buffer.clear();          // Réinitialiser le tampon pour la prochaine saisie

        qDebug() << "ID complet reçu :" << userID;

        // Validation de l'ID
        QString message;
        if (isValidUser(userID)) {
            serial.write("VALID\n");
            message = "ID valide. Réponse envoyée.";
            qDebug() << message;

            // Afficher une boîte de dialogue une seule fois
            static bool dialogShown = false; // Indicateur pour éviter des affichages multiples
            if (!dialogShown) {
                dialogShown = true;
                QMessageBox::information(nullptr, "Validation de l'ID utilisateur", message);
                dialogShown = false; // Réinitialiser pour autoriser une nouvelle boîte de dialogue
            }
        } else {
            serial.write("INVALID\n");
            message = "ID invalide. Réponse envoyée.";
            qDebug() << message;
        }
    } else if (buffer.length() > 4) {
        // Gérer les débordements (par exemple, si plus de 4 caractères sont reçus accidentellement)
        qDebug() << "Tampon débordé. Réinitialisation du tampon.";
        buffer.clear();
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
