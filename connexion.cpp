#include "connexion.h"

Connexion::Connexion()
{

}
bool Connexion::createconnect() {
    // Vérifiez si la connexion par défaut existe déjà
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        qDebug() << "Connexion à la base de données déjà établie.";
        return true; // La connexion existe déjà, pas besoin de la recréer
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "qt_sql_default_connection"); // Nom de la connexion
    db.setDatabaseName("projetcpp2a32");
    db.setUserName("projetcpp2a32"); // insérer nom de l'utilisateur
    db.setPassword("mp2a32"); // insérer mot de passe de cet utilisateur

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données:" << db.lastError().text();
        return false;
    }

    qDebug() << "Connexion à la base de données réussie.";
    return true;
}
