#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QComboBox>

#include <QMainWindow>
#include "client.h"
#include"connexion.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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
    bool validateForm();


    void on_comboBoxt_currentIndexChanged(int index);


// Déclarez cette fonction ici

    void on_exportButton_clicked();














private:
    Ui::MainWindow *ui;
    client C;
    void afficherDonnees();
};
#endif // MAINWINDOW_H






