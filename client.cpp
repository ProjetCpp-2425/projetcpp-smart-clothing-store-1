
#include"connexion.h"
#include "client.h"
#include <QtDebug>
#include<QSqlError>
#include<QObject>
#include<QMessageBox>
#include<map>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include <QPdfWriter>
#include <QPainter>
#include <QSqlRecord>
#include <QFileDialog>
client::client()
{
    IDC=0; NOM=""; PRENOM=""; EMAIL=0; ADRESSE="";
}
client::client(int IDC,QString NOM,QString PRENOM,QString EMAIL, QString ADRESSE )
{this->IDC=IDC; this->NOM=NOM; this->PRENOM=PRENOM;this->EMAIL=EMAIL;this->ADRESSE=ADRESSE;}

QString client:: getEMAIL(){return EMAIL;}
QString client:: getNOM(){return NOM;}
QString client::getPRENOM(){return PRENOM;}
QString client::getADRESSE(){return ADRESSE;}
int client::getIDC(){return IDC;}
void client::setEMAIL(QString EMAIL){this->EMAIL=EMAIL;}
void client::setNOM(QString NOM){this->NOM=NOM;}
void client::setPRENOM(QString PRENOM){this->PRENOM=PRENOM;}
void client::setADRESSE(QString ADRESSE){this->ADRESSE=ADRESSE;}
void client::setIDC(int IDC){this->IDC=IDC;}


bool client::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO CLIENT (IDC, NOM, PRENOM, EMAIL, ADRESSE) "
                  "VALUES (:IDC, :NOM, :PRENOM, :EMAIL, :ADRESSE)");

    query.bindValue(":IDC", IDC);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":EMAIL", EMAIL); // Assurez-vous que EMAIL est une chaine
    query.bindValue(":ADRESSE", ADRESSE);

    qDebug() << query.lastQuery(); // Afficher la requête préparée pour le débogage

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'insertion : " << query.lastError().text();
        return false; // Échec de l'insertion
    }

    return true; // Insertion réussie
}




QSqlQueryModel* client::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Définir la requête SQL pour obtenir les données de la table CLIENT
    model->setQuery("SELECT * FROM CLIENT");

    // Définir les en-têtes de colonnes pour l'affichage
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));

    return model;
}



bool client::supprimer(int IDC) {
    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "La base de données n'est pas ouverte. Vérifiez la connexion.";
        return false;
    }

    QSqlQuery query;







            // Vérifiez si l'identifiant existe
            query.prepare("SELECT COUNT(*) FROM CLIENT WHERE IDC = :IDC");
            query.bindValue(":IDC", IDC);

            if (!query.exec()) {
                qDebug() << "Erreur lors de la vérification de l'existence de l'identifiant : " << query.lastError().text();
                return false;
            }

            query.next();
            int count = query.value(0).toInt();
            qDebug() << "Count for IDC" << IDC << ":" << count;

            if (count == 0) {
                qDebug() << "IDC non trouvé : " << IDC;
                return false;
            }

            // Si l'identifiant existe, procédez à la suppression
            query.prepare("DELETE FROM CLIENT WHERE IDC = :IDC");
            query.bindValue(":IDC", IDC);

            if (!query.exec()) {
                qDebug() << "Erreur lors de la suppression : " << query.lastError().text();
                return false;
            }

            return true;
        }

bool client::Modifier(int IDC, QString NOM, QString PRENOM, QString EMAIL, QString ADRESSE)
{
    QSqlQuery query;
    query.prepare("UPDATE Client SET NOM=:NOM, PRENOM=:PRENOM, EMAIL=:EMAIL, ADRESSE=:ADRESSE WHERE IDC=:IDC");
    query.bindValue(":IDC", IDC);  // Pas besoin de convertir en QString, Qt gère cela.
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":EMAIL", EMAIL);
    query.bindValue(":ADRESSE", ADRESSE);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du client:" << query.lastError().text();
        return false;
    }

    return true;
}


void client::rechercher(const QString &a, QTableView *g)
{
    QSqlQuery qry;
                    QSqlQueryModel *m = new QSqlQueryModel(); //Alloue dynamiquement un modèle QSqlQueryModel qui servira à contenir les résultats de la requête SQL.
                    qry.prepare("SELECT * FROM Client WHERE NOM LIKE :searchTerm OR PRENOM LIKE :searchTerm OR  EMAIL LIKE :searchTerm OR ADRESSE LIKE :searchTerm");
                    qry.bindValue(":searchTerm", a + "%");  // Utilisation du '%' pour la recherche par début de mot

                    if (qry.exec()) {
                        m->setQuery(qry); //Charge les résultats de la requête qry dans le modèle m (objet QSqlQueryModel).
                        g->setModel(m);  // Mise à jour du modèle de la table
                    } else {
                        qDebug() << "Erreur lors de la recherche : " << qry.lastError().text();
                    }
}



QSqlQueryModel* client::trierclient(const QString& critere)
                       {
                           QSqlQueryModel *model = new QSqlQueryModel();

                           // Création de la requête SQL en fonction du critère passé
                           QString query;
                           if (critere == "NOM") {
                               query = "SELECT * FROM CLIENT ORDER BY NOM ASC";  //Construit une chaîne de requête SQL qui sélectionne tous les enregistrements de la table CLIENT, triés par la colonne spécifiée dans critere, en ordre croissant (ASC).
                           } else if (critere == "PRENOM") {
                               query = "SELECT * FROM CLIENT ORDER BY PRENOM ASC";
                           } else if (critere == "IDC") {
                               query = "SELECT * FROM CLIENT ORDER BY IDC ASC";
                           }

                           // Appliquer la requête à la base de données
                           model->setQuery(query); //Exécute la requête SQL construite et lie les résultats au modèle model.

                           // Définir les en-têtes de colonnes pour l'affichage
                           model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC"));
                           model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
                           model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
                           model->setHeaderData(3, Qt::Horizontal, QObject::tr("ADRESSE"));
                           model->setHeaderData(4, Qt::Horizontal, QObject::tr("EMAIL"));

                           return model;
                       }





bool client::exporterPDF()
{
    // Choisir l'emplacement de sauvegarde
    QString filename = QFileDialog::getSaveFileName(nullptr, "Enregistrer en PDF", "", "*.pdf");
    if (filename.isEmpty())
        return false;
//nullptr : Pas de parent pour la boîte de dialogue.
    //"Enregistrer en PDF" : Titre de la boîte de dialogue.
    //"" : Dossier de départ vide (par défaut, ouvre le dernier répertoire utilisé ou celui du système).
    //"*.pdf" : Filtre pour n'afficher que les fichiers PDF.
    if (QFileInfo(filename).suffix().isEmpty())
        filename.append(".pdf");
//Vérifie si l'utilisateur n'a pas sélectionné de fichier (par exemple, s'il a annulé la boîte de dialogue).
// Si filename est vide, la fonction retourne false, indiquant que l'exportation a échoué ou été annulée.

    // Création du document PDF
    QPdfWriter pdfWriter(filename);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    int yPos = 50;

    // Titre du document
    painter.setFont(QFont("Helvetica", 18, QFont::Bold));
    painter.drawText(2000, yPos, "Liste des Clients");
    yPos += 200;

    // En-tête du tableau
    painter.setFont(QFont("Helvetica", 12, QFont::Bold));
    painter.drawText(100, yPos, "IDC");
    painter.drawText(1000, yPos, "NOM");
    painter.drawText(2000, yPos, "PRENOM");
    painter.drawText(3000, yPos, "EMAIL");
    painter.drawText(4000, yPos, "ADRESSE");
    yPos += 200;

    // Requête SQL pour récupérer tous les clients
    QSqlQuery query;
    query.prepare("SELECT IDC, NOM, PRENOM, EMAIL, ADRESSE FROM Client");

    if (query.exec()) {
        // Remplissage du tableau avec les données des clients
        painter.setFont(QFont("Helvetica", 10));
        while (query.next()) {
            QString identifiant = query.value("IDC").toString();
            QString nom = query.value("NOM").toString();
            QString prenom = query.value("PRENOM").toString();
            QString email = query.value("EMAIL").toString();
            QString adresse = query.value("ADRESSE").toString();

            painter.drawText(100, yPos, identifiant);
            painter.drawText(1000, yPos, nom);
            painter.drawText(2000, yPos, prenom);
            painter.drawText(3000, yPos, email);
            painter.drawText(4000, yPos, adresse);
            yPos += 200;
        }
    } else {
        qWarning() << "Erreur de requête : " << query.lastError().text();
        return false;
    }

    painter.end(); // Fin du dessin et enregistrement du PDF

    return true;
}


































