#include "arduino.h"
#include "qregularexpression.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent), serialPort(new QSerialPort(this)), connected(false)
{
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::readData);  // Connexion du signal readyRead à readData
}

Arduino::~Arduino()
{
    if (serialPort->isOpen()) {
        serialPort->close();  // Fermeture de la connexion série lors de la destruction de l'objet
    }
}

bool Arduino::openConnection(const QString &portName, int baudRate)
{
    if (connected) {
        qDebug() << "Déjà connecté à un port série.";
        return true;  // Retourne vrai si déjà connecté
    }

    serialPort->setPortName("COM4");  // Définit le port série
    serialPort->setBaudRate(QSerialPort::Baud9600);  // Définit le taux de transmission
    serialPort->setDataBits(QSerialPort::Data8);  // Définit la longueur des données
    serialPort->setParity(QSerialPort::NoParity);  // Aucune parité
    serialPort->setStopBits(QSerialPort::OneStop);  // Un seul bit de stop
    serialPort->setFlowControl(QSerialPort::NoFlowControl);  // Aucun contrôle de flux

    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connexion série établie sur" << portName;
        return true;  // Connexion réussie
    } else {
        qDebug() << "Échec de la connexion série sur" << portName;
        return false;  // Échec de la connexion
    }
}

void Arduino::closeConnection()
{
    if (serialPort->isOpen()) {
        serialPort->close();  // Fermeture du port série
        connected = false;
        qDebug() << "Connexion série fermée.";
    }
}

bool Arduino::isConnected() const
{
    return connected;  // Retourne vrai si le port est ouvert
}
/*
void Arduino::readData()
{
    QByteArray data = serialPort->readAll();  // Lire toutes les données disponibles

    emit dataReceived(data);  // Émettre le signal avec les données lues
}
*/

void Arduino::readData() {
    QByteArray data = serialPort->readAll();
    QString message(data);

    // Nettoyer la chaîne pour enlever les caractères inutiles
    message = message.trimmed();
    message.replace("\r", "").replace("\n", "");  // Supprime les retours chariot et les nouvelles lignes

    // Vérifiez si le message contient l'UID complet
    QRegularExpression uidRegex("UID de la carte :\\s*([0-9A-F]{8})");
    QRegularExpressionMatch match = uidRegex.match(message);

    if (match.hasMatch()) {
        QString formattedUid = match.captured(1).toUpper();  // Extraire l'UID et le formater en majuscules
        qDebug() << "UID reçu de l'Arduino : " << formattedUid;
        emit clientFound(formattedUid);
    } else {
        // Affiche un message de débogage et tente de trouver une partie valide de l'UID
        qDebug() << "UID non trouvé dans la chaîne reçue : " << message;
        // Vous pouvez également essayer d'extraire une partie de l'UID s'il y a des fragments
        if (message.contains("UID de la carte :")) {
            QString partialUid = message.split("UID de la carte :").last();
            if (partialUid.length() >= 8) {
                QString extractedUid = partialUid.left(8).toUpper();
                qDebug() << "UID partiel extrait : " << extractedUid;
                emit clientFound(extractedUid);
            }
        }
    }
}

void Arduino::writeData(const QByteArray &data)
{
    if (serialPort->isOpen() && isConnected()) {
        serialPort->write(data);  // Écrit les données dans le port série
        serialPort->flush();      // S'assure que les données sont transmises immédiatement
    } else {
        qDebug() << "Le port série n'est pas ouvert ou connecté.";
    }
}
