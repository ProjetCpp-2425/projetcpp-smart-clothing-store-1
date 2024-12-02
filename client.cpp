
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
    IDC=0; NOM=""; PRENOM=""; EMAIL=""; ADRESSE="" ; TELEPHONE=0;
}
client::client(int IDC,QString NOM,QString PRENOM,QString EMAIL, QString ADRESSE, int TELEPHONE )
{this->IDC=IDC; this->NOM=NOM; this->PRENOM=PRENOM;this->EMAIL=EMAIL;this->ADRESSE=ADRESSE;this->TELEPHONE=TELEPHONE;}

QString client:: getEMAIL(){return EMAIL;}
QString client:: getNOM(){return NOM;}
QString client::getPRENOM(){return PRENOM;}
QString client::getADRESSE(){return ADRESSE;}
int client::getTELEPHONE(){return TELEPHONE;}
int client::getIDC(){return IDC;}
void client::setEMAIL(QString EMAIL){this->EMAIL=EMAIL;}
void client::setNOM(QString NOM){this->NOM=NOM;}
void client::setPRENOM(QString PRENOM){this->PRENOM=PRENOM;}
void client::setADRESSE(QString ADRESSE){this->ADRESSE=ADRESSE;}
void client::setTELEPHONE(int TELEPHONE){this->TELEPHONE=TELEPHONE;}

void client::setIDC(int IDC){this->IDC=IDC;}


bool client::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO CLIENT (IDC, NOM, PRENOM, EMAIL, ADRESSE,TELEPHONE) "
                  "VALUES (:IDC, :NOM, :PRENOM, :EMAIL, :ADRESSE,:TELEPHONE)");

    query.bindValue(":IDC", IDC);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":EMAIL", EMAIL); // Assurez-vous que EMAIL est une chaine
    query.bindValue(":ADRESSE", ADRESSE);
     query.bindValue(":TELEPHONE", TELEPHONE);

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
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("POINTF"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("TELEPHONE"));


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

bool client::Modifier(int IDC, QString NOM, QString PRENOM, QString EMAIL, QString ADRESSE,int TELEPHONE)
{
    QSqlQuery query;
    query.prepare("UPDATE Client SET NOM=:NOM, PRENOM=:PRENOM, EMAIL=:EMAIL, ADRESSE=:ADRESSE, TELEPHONE=:TELEPHONE WHERE IDC=:IDC");
    query.bindValue(":IDC", IDC);  // Pas besoin de convertir en QString, Qt gère cela.
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":EMAIL", EMAIL);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":TELEPHONE", TELEPHONE);


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
                    qry.prepare("SELECT * FROM Client WHERE NOM LIKE :searchTerm OR PRENOM LIKE :searchTerm OR  EMAIL LIKE :searchTerm OR ADRESSE LIKE :searchTerm OR  TELEPHONE LIKE :searchTerm ");
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
                           model->setHeaderData(5, Qt::Horizontal, QObject::tr("TELEPHONE"));

                           return model;
}

/**/




bool client::exporterPDF()
{
    // Choisir l'emplacement de sauvegarde
    QString filename = QFileDialog::getSaveFileName(nullptr, "Enregistrer en PDF", "", "*.pdf");
    //Le filtre *.pdf s'assure que seul le format PDF est sélectionné.
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
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));  //Configuration de la taille de la page
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
    painter.drawText(5000, yPos, "TELEHONE");

    yPos += 200;

    // Requête SQL pour récupérer tous les clients
    QSqlQuery query;
    query.prepare("SELECT IDC, NOM, PRENOM, EMAIL, ADRESSE , TELEPHONE FROM Client");

    if (query.exec()) {
        // Remplissage du tableau avec les données des clients
        painter.setFont(QFont("Helvetica", 10));
        while (query.next()) {
            QString identifiant = query.value("IDC").toString();
            QString nom = query.value("NOM").toString();
            QString prenom = query.value("PRENOM").toString();
            QString email = query.value("EMAIL").toString();
            QString adresse = query.value("ADRESSE").toString();
            QString telephone = query.value("TELEPHONE").toString();
//Affichage de chaque ligne de résultat

            painter.drawText(100, yPos, identifiant);
            painter.drawText(1000, yPos, nom);
            painter.drawText(2000, yPos, prenom);
            painter.drawText(3000, yPos, email);
            painter.drawText(4000, yPos, adresse);
            painter.drawText(5000, yPos, telephone);

            yPos += 200;
        }
    } else {
        qWarning() << "Erreur de requête : " << query.lastError().text();
        return false;
    }

    painter.end(); // Fin du dessin et enregistrement du PDF

    return true;
}





/**/


int client::calculerPoints(int IDC)
{
    QSqlQuery qry;
    int nbr = 0;
    bool test;

    // Préparer la requête pour compter le nombre d'achats
    qry.prepare("SELECT COUNT(*) FROM CLIENT, ACHETER WHERE CLIENT.IDC = ACHETER.IDC AND CLIENT.IDC = :IDC");
    qry.bindValue(":IDC", IDC);

    if (qry.exec() && qry.next()) {
        nbr = qry.value(0).toInt();  // Récupérer directement la première colonne (index 0)
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Échec de la récupération des points.\n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        return -1;  // Retourner une valeur spéciale pour indiquer l'échec
    }

    // Mise à jour des points dans la table CLIENT
    qry.prepare("UPDATE CLIENT SET POINTF = :nbr WHERE IDC = :IDC");
    qry.bindValue(":nbr", nbr);
    qry.bindValue(":IDC", IDC);
    test = qry.exec();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Points ajoutés avec succès.\n"
                                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Échec de l'ajout des points.\n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    return nbr;
}








/**/

bool client::remise(int IDC)
{
    QSqlQuery qry;
    int points = 0;
    QString remiseAvecPourcentage;

    // Récupérer les points fidélité du client
    qry.prepare("SELECT POINTF FROM CLIENT WHERE IDC = :IDC");
    qry.bindValue(":IDC", IDC);  // IDENTIFIANT est maintenant un entier
    if (!qry.exec()) {
        qDebug() << "Erreur dans la requête SELECT:" << qry.lastError().text();
        return false;
    }
//Si le client n’existe pas ou n’a pas de points, la fonction retourne false.
    if (qry.next()) {
        points = qry.value("POINTF").toInt();
    } else {
        qDebug() << "Client introuvable avec IDC:" << IDC;
        return false;
    }

    // Calcul de la remise basée sur les points fidélité
    //Calcule la remise : 10% pour chaque palier de 5 points, plafonné à 100%.

    double remise = 0.0;
    if (points >= 5) {
        remise = (points / 5) * 10; // Chaque palier de 5 points ajoute 10% de remise
        if (remise > 100) remise = 100; // Plafonner la remise à 100%
    } else {
        qDebug() << "Pas assez de points pour une remise.";
        return false;
    }

    // Ajouter le symbole % à la remise
    remiseAvecPourcentage = QString("%1%").arg(QString::number(remise, 'f', 2));

    // Mise à jour de la remise dans la base de données
    qry.prepare("UPDATE CLIENT SET REMISE = :remise WHERE IDC = :IDC");
    qry.bindValue(":remise", remiseAvecPourcentage); // Insère "10%" ou "20%"
    qry.bindValue(":IDC", IDC);  // IDENTIFIANT est maintenant un entier

    if (!qry.exec()) {
            qDebug() << "Erreur dans la requête UPDATE:" << qry.lastError().text();
            return false;
        }

        return true;
    }
std::map<QString,int> client::statNbrPerType()
{
    QSqlQuery query;
    std::map <QString,int> list;
    try {
        query.prepare("SELECT SUM(POINTF) ,IDC from CLIENT GROUP BY IDC");
        //Exécute la requête, lit les résultats et remplit la carte avec les valeurs.
        if(query.exec()){
            while (query.next()) {
                query.value('s').toInt();
                list.insert({query.value(1).toString() , query.value(0).toInt()});
            }
        return list;
        }

//Si une erreur survient, retourne une carte contenant une entrée "error".
    } catch (...) {
        list["error"] = 0;
        return  list;
    }

}














