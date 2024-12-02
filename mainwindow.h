#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QComboBox>

#include <QMainWindow>
#include "client.h"
#include"connexion.h"


#include "arduino.h"
#include <QSerialPort>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void on_pushButton_15_clicked();
    void on_pushButton_13_clicked();
    void onClientSelected(const QModelIndex &index);


    void on_pushButton_modif_clicked();


    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_line_recherche_textChanged(const QString &text);
    void validateId();         // Validation pour le champ ID
    void validateName();       // Validation pour Nom et Prénom
    void validateEmail(); // Validation pour Email
    void validateAddress();
 /**/   void validateTELEPHONE();

    bool validateForm();


    void on_comboBoxt_currentIndexChanged(int index);


// Déclarez cette fonction ici

    void on_exportButton_clicked();

    void on_notifications_clicked();  // Slot pour votre bouton
    //void notifySMS(const QString &TELEPHONE, const QString &message);
    /**/   /* void on_remise_clicked(); */



    void notifySMS(int TELEPHONE, const QString &message) ;

    void on_nbpoints_clicked();
    void sendSMS(const QString &clientPhoneNumber, const QString &messageBody);
void on_pb_stat_clicked();
void on_remise_clicked();
void onClientFound(const QString &UIDC);
void writeDataToArduino(const QByteArray &data);
void initSerialPort();






private:
    Ui::MainWindow *ui;
    client C;
    void afficherDonnees();
    Arduino *arduino;  // Déclarez un objet Arduino pour gérer la communication
    /*QString arduinoPortName;      // Nom du port série
    void setupArduino();          // Méthode d'initialisation Arduino
    void readFromArduino();       // Lecture des données Arduino
    void handleUID(const QString &UIDC);
    void handleData(const QByteArray &data);*/
    // Traitement des données Arduino (exemple : UID)
    QString arduinoPortName;      // Nom du port série
       void setupArduino();          // Méthode d'initialisation Arduino
       void readFromArduino();       // Lecture des données Arduino
      // void handleUID(const QString &UIDC);
       //void handleData(const QByteArray &data);
       void readSerialData();
       void handleData(const QByteArray &data);




};
#endif // MAINWINDOW_H

