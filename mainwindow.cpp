
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "client.h"
#include"QSortFilterProxyModel"
#include <QGroupBox>
#include "connexion.h"
#include <QApplication>
#include <QSqlQuery>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBoxt->addItem("NOM");
    ui->comboBoxt->addItem("PRENOM");
     ui->comboBoxt->addItem("IDC");

            // Connecter le QComboBox au slot pour gérer le tri
        connect(ui->comboBoxt, &QComboBox::currentIndexChanged, this, &MainWindow::on_comboBoxt_currentIndexChanged);
        connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::on_exportButton_clicked);

    // Connexion des champs pour la validation manuelle
    connect(ui->textEdit_id, &QTextEdit::textChanged, this, &MainWindow::validateId);
    connect(ui->textEdit_nom, &QTextEdit::textChanged, this, &MainWindow::validateName);
    connect(ui->textEdit_prenom, &QTextEdit::textChanged, this, &MainWindow::validateName);
    connect(ui->textEdit_mail, &QTextEdit::textChanged, this, &MainWindow::validateEmail);



    // Autres initialisations
    Connexion conn;
    if (!conn.createconnect()) {
        qDebug() << "Erreur : Connexion à la base de données échouée.";
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base de données.");
        return;
    }

    ui->tab_client->setModel(C.afficher());

    // Connexions pour les boutons
    connect(ui->pushButton_modif, &QPushButton::clicked, this, &MainWindow::on_pushButton_modif_clicked);
    connect(ui->tab_client, &QTableView::clicked, this, &MainWindow::onClientSelected);
    connect(ui->line_recherche, &QLineEdit::textChanged, this, &MainWindow::on_line_recherche_textChanged);
}


void MainWindow::validateId() {
    QString text = ui->textEdit_id->toPlainText();
    QRegularExpression regex("^[0-9]+$");  // Expression régulière pour valider uniquement les chiffres

    if (!regex.match(text).hasMatch()) {
        ui->textEdit_id->setStyleSheet("border: 2px solid red;");  // Mise en rouge si non valide
    } else {
        ui->textEdit_id->setStyleSheet("");  // Réinitialiser si valide
    }
}


void MainWindow::validateName() {
    QTextEdit *senderField = qobject_cast<QTextEdit *>(sender());
    if (!senderField) return;

    QString text = senderField->toPlainText();
    QRegularExpression regex("^[A-Za-zÀ-ÿ\\s]*$");

    if (!regex.match(text).hasMatch()) {
        senderField->setStyleSheet("border: 2px solid red;");
    } else {
        senderField->setStyleSheet("");
    }
}

void MainWindow::validateEmail() {
    QString text = ui->textEdit_mail->toPlainText();
    QRegularExpression regex("^[\\w\\.-]+@[\\w-]+(\\.[\\w-]+)*\\.[A-Za-z]{2,}$");

    if (!regex.match(text).hasMatch()) {
        ui->textEdit_mail->setStyleSheet("border: 2px solid red;");
    } else {
        ui->textEdit_mail->setStyleSheet("");
    }
}

void MainWindow::validateAddress() {
    QString text = ui->textEdit_adresse->toPlainText();
    QRegularExpression regex("^[A-Za-zÀ-ÿ\\s]*$");  // Lettres uniquement + espaces
    const int maxLength = 30;  // Longueur maximale fixée à 30 caractères

    if (!regex.match(text).hasMatch() || text.length() > maxLength) {
        ui->textEdit_adresse->setStyleSheet("border: 2px solid red;");
        QMessageBox::warning(this, "Erreur", QString("L'adresse doit contenir uniquement des lettres et ne pas dépasser %1 caractères.").arg(maxLength));

        // Limiter la longueur si nécessaire
        if (text.length() > maxLength) {
            ui->textEdit_adresse->setPlainText(text.left(maxLength));
            QTextCursor cursor = ui->textEdit_adresse->textCursor();
            cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
            ui->textEdit_adresse->setTextCursor(cursor);
        }
    } else {
        ui->textEdit_adresse->setStyleSheet("");  // Réinitialiser le style si valide
    }
}


bool MainWindow::validateForm() {
    bool isValid = true;

    // Vérification de l'ID (uniquement des chiffres)
    QString idText = ui->textEdit_id->toPlainText();
    QRegularExpression regexId("^[0-9]+$");  // ID doit contenir uniquement des chiffres
    if (!regexId.match(idText).hasMatch() || idText.isEmpty()) {
        ui->textEdit_id->setStyleSheet("border: 2px solid red;");
        isValid = false;
    } else {
        ui->textEdit_id->setStyleSheet("");
    }

    // Vérification des noms et prénoms (lettres avec accents et espaces)
    QString nomText = ui->textEdit_nom->toPlainText();
    QString prenomText = ui->textEdit_prenom->toPlainText();
    QRegularExpression regexName("^[A-Za-zÀ-ÿ\\s]*$");

    if (nomText.isEmpty() || !regexName.match(nomText).hasMatch()) {
        ui->textEdit_nom->setStyleSheet("border: 2px solid red;");
        isValid = false;
    } else {
        ui->textEdit_nom->setStyleSheet("");
    }

    if (prenomText.isEmpty() || !regexName.match(prenomText).hasMatch()) {
        ui->textEdit_prenom->setStyleSheet("border: 2px solid red;");
        isValid = false;
    } else {
        ui->textEdit_prenom->setStyleSheet("");
    }

    // Vérification de l'email (format d'email valide)
    QString emailText = ui->textEdit_mail->toPlainText();
    QRegularExpression regexEmail("^[\\w\\.-]+@[\\w-]+(\\.[\\w-]+)*\\.[A-Za-z]{2,}$");
    if (emailText.isEmpty() || !regexEmail.match(emailText).hasMatch()) {
        ui->textEdit_mail->setStyleSheet("border: 2px solid red;");
        isValid = false;
    } else {
        ui->textEdit_mail->setStyleSheet("");
    }

    // Vérification de l'adresse (lettres, accents et espaces uniquement, longueur maximale)
    QString adresseText = ui->textEdit_adresse->toPlainText();
    QRegularExpression regexAddress("^[A-Za-zÀ-ÿ\\s]{1,30}$");  // Limite de 30 caractères
    if (adresseText.isEmpty() || !regexAddress.match(adresseText).hasMatch()) {
        ui->textEdit_adresse->setStyleSheet("border: 2px solid red;");
        isValid = false;
        //QMessageBox::warning(this, "Erreur", "L'adresse doit contenir uniquement des lettres et des espaces, et ne pas dépasser 30 caractères.");
    } else {
        ui->textEdit_adresse->setStyleSheet("");
    }

    // Message général si des champs sont invalides
    if (!isValid) {
        QMessageBox::warning(this, "Erreur", "Veuillez corriger les champs en rouge avant de soumettre.");
    }

    return isValid;
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_15_clicked()
{    if (validateForm()) {

        int IDC = ui->textEdit_id->toPlainText().toInt();
        QString NOM = ui->textEdit_nom->toPlainText();
        QString PRENOM = ui->textEdit_prenom->toPlainText();
        QString EMAIL = ui->textEdit_mail->toPlainText();
        QString ADRESSE = ui->textEdit_adresse->toPlainText();
/*
        // Validation des champs
        if (NOM.isEmpty() || PRENOM.isEmpty() || EMAIL.isEmpty() || ADRESSE.isEmpty() || IDC <= 0) {
            QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                                 QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
            return; // Sortie de la fonction si des champs sont vides
        }
        */

        client C(IDC, NOM, PRENOM, EMAIL, ADRESSE);
        bool test = C.ajouter();

        if (test)
        {
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué.\n"
                                                 "Cliquez sur Annuler pour sortir."), QMessageBox::Cancel);
            ui->tab_client->setModel(C.afficher());
            // Vérification de la non-vacuité des champs avant d'effacer
            if (!ui->textEdit_nom->toPlainText().isEmpty() ||
                !ui->textEdit_prenom->toPlainText().isEmpty() ||
                !ui->textEdit_adresse->toPlainText().isEmpty() ||
                !ui->textEdit_mail->toPlainText().isEmpty() ||
                !ui->textEdit_id->toPlainText().isEmpty())
            {
                ui->textEdit_nom->clear();
                ui->textEdit_prenom->clear();
                ui->textEdit_adresse->clear();
                ui->textEdit_mail->clear();
                ui->textEdit_id->clear();
            }
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Ajout non effectué.\n"
                                              "Cliquez sur Annuler pour sortir."), QMessageBox::Cancel);
        }
    }
    }



void MainWindow::on_pushButton_13_clicked()
{





    QString idText = ui->idsupp->toPlainText();

    int IDC = idText.toInt();

    if (idText.isEmpty()) {
        QMessageBox::warning(nullptr, QObject::tr("Attention"),
                             QObject::tr("Veuillez entrer un identifiant."), QMessageBox::Cancel);
        return; // Sortir de la fonction si l'identifiant est vide
    }

    client C;
    C.setIDC(IDC);
    bool test = C.supprimer(C.getIDC());

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Delete successful.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->tab_client->setModel(C.afficher());
        ui->idsupp->clear();


    } else {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Delete failed.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::onClientSelected(const QModelIndex &index) {
    // Récupère l'identifiant du client sélectionné
    int clientID = ui->tab_client->model()->data(index).toInt();

    // Préparez une requête pour récupérer les informations du client depuis la base de données
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, EMAIL, ADRESSE FROM Client WHERE IDC = :IDC");
    query.bindValue(":IDC", clientID);

    if (query.exec() && query.next()) {
        // Remplir les champs de l'interface avec les données du client
        ui->textEdit_nom->setText(query.value("NOM").toString());
        ui->textEdit_prenom->setText(query.value("PRENOM").toString());
        ui->textEdit_mail->setText(query.value("EMAIL").toString());
        ui->textEdit_adresse->setText(query.value("ADRESSE").toString());
        ui->textEdit_id->setText(QString::number(clientID));  // Convertir en QString si nécessaire

        ui->textEdit_id->setReadOnly(true);
    } else {
        qDebug() << "Erreur lors de la récupération des informations du client:" << query.lastError().text();
    }
}





void MainWindow::on_pushButton_modif_clicked()
{
    // Flag pour vérifier si la modification a réussi
    bool isModificationSuccessful = false;

    // Validation du formulaire (Si ça échoue, on ne va pas plus loin)
    if (!validateForm()) {
        return;  // Sortie si la validation échoue
    }

    bool idOk; // pour vérifier si la conversion a réussi
    int IDC = ui->textEdit_id->toPlainText().toInt(&idOk);
//Cela permet une validation robuste des entrées utilisateur sans provoquer de plantage ou d'erreur si la saisie n'est pas valide.

        QString NOM = ui->textEdit_nom->toPlainText();
        QString PRENOM = ui->textEdit_prenom->toPlainText();
        QString EMAIL = ui->textEdit_mail->toPlainText();
        QString ADRESSE = ui->textEdit_adresse->toPlainText();

        // Validation des entrées
        if (!idOk || IDC <= 0) {
            QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                                 QObject::tr("Veuillez entrer un ID client valide."), QMessageBox::Ok);
            return; // Sortie de la fonction si l'ID n'est pas valide
        }

        if (NOM.isEmpty() || PRENOM.isEmpty() || EMAIL.isEmpty() || ADRESSE.isEmpty()) {
            QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                                 QObject::tr("Veuillez remplir tous les champs."), QMessageBox::Ok);
            return; // Sortie de la fonction si des champs sont vides
        }

        client C;
        bool test = C.Modifier(IDC, NOM, PRENOM, EMAIL, ADRESSE);

        if (test) {
            // Si la modification réussit
            isModificationSuccessful = true;
            qDebug() << "Modification réussie pour l'ID" << IDC;  // Débogage
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Modification réussie.\nCliquez sur Annuler pour sortir."), QMessageBox::Cancel);

            // Actualisation du modèle dans la table
            ui->tab_client->setModel(C.afficher());

            // Déverrouiller l'ID pour futures modifications
            ui->textEdit_id->setReadOnly(false);

            // Effacer les champs après modification réussie
            ui->textEdit_nom->clear();
            ui->textEdit_prenom->clear();
            ui->textEdit_adresse->clear();
            ui->textEdit_mail->clear();
            ui->textEdit_id->clear();

            // Réinitialisation des styles des champs
            ui->textEdit_nom->setStyleSheet("");
            ui->textEdit_prenom->setStyleSheet("");
            ui->textEdit_mail->setStyleSheet("");
            ui->textEdit_adresse->setStyleSheet("");
            ui->textEdit_id->setStyleSheet("");

        } else {
            // Si la modification échoue
            qDebug() << "Échec de la modification pour l'ID" << IDC;  // Débogage
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                                  QObject::tr("La modification a échoué.\nCliquez pour sortir."), QMessageBox::Cancel);
        }

        // La validation n'est plus nécessaire ici car la modification a réussi.
    }








    void MainWindow::on_line_recherche_textChanged(const QString &text)
    {
        //si le champ est vide ,afficher tous les clients
        if (text.isEmpty()){
            client C ;
            ui->tab_client->setModel(C.afficher());
            return ;
        }

        //appel a la methode rechercher pour mettre à jour le tableau avec les resultats
       client C ;
       C.rechercher(text,ui->tab_client);


    }





    void MainWindow::on_comboBoxt_currentIndexChanged(int index)
    {

        QString critere;

             // Obtenir le critère sélectionné dans le QComboBox
             if (index == 0) {
                 critere = "NOM";
             } else if (index == 1) {
                 critere = "PRENOM";
             } else if (index == 2) {
                 critere = "IDC";
             }

             // Appeler la fonction de tri depuis la classe client
             client c;
             QSqlQueryModel *model = c.trierclient(critere);

             // Afficher le résultat du tri dans la vue
             ui->tab_client->setModel(model);

    }




    void MainWindow::on_exportButton_clicked()
    {
        client c;
         if (c.exporterPDF()) {
          QMessageBox::information(this, "Exportation PDF", "Le fichier PDF a été créé avec succès.");
            } else {
                QMessageBox::warning(this, "Erreur", "Impossible de créer le fichier PDF.");
            }
    }



















