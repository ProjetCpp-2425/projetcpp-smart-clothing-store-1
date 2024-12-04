#include "employe.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <cstddef>
employe::employe() : id(0), date(QDate::currentDate()), salaire(0) {}


employe::employe(int id,QString prenom,QString nom,QDate date,QString poste,int salaire) {
    this->id=id;
    this->prenom=prenom;
    this->nom=nom;
    this->date=date;
    this->poste=poste;
    this->salaire=salaire;
}

bool employe::ajouter(){

    QSqlQuery query;
    QString res= QString::number(id);
    //QString s=QString::number(salaire);

    query.prepare("INSERT INTO EMPLOYES (ID_EMPLOYE, NOM, PRENOM,DATE_EMBAUCHE ,POSTE ,SALAIRE) VALUES (:id, :nom, :prenom, :date , :poste, :salaire)");


    // varibale
    query.bindValue(":id",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":date", date);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);


    bool success = query.exec();
    if (!success) {
        qDebug() << "Error inserting into table:" << query.lastError().text();
    }
    return success;






}

bool employe::supprimer(int id){
    QSqlQuery query;
    QString res=QString::number(id);

    query.prepare("Delete from EMPLOYES where ID_EMPLOYE= :id");
    query.bindValue(":id",res);
    return query.exec();



}

QSqlQueryModel * employe::afficher(){
    QSqlQueryModel * model=new QSqlQueryModel();
    model->setQuery("select * from EMPLOYES");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("PRENOM"));


    return model;





}

bool employe::modifier(int id, QString nom, QString prenom, QDate date, QString poste, int salaire) {
    QSqlQuery query;
    QString update = "UPDATE EMPLOYES SET ";
    bool first = true;


    if (!nom.isEmpty()) {
        if (!first) update += ", ";
        update += "NOM = :nom";
        first = false;
    }
    if (!prenom.isEmpty()) {
        if (!first) update += ", ";
        update += "PRENOM = :prenom";
        first = false;
    }
    if (date.isValid() && !date.isNull()) {
        if (!first) update += ", ";
        update += "DATE_EMBAUCHE = :date";
        first = false;
    }
    if (!poste.isEmpty()) {
        if (!first) update += ", ";
        update += "POSTE = :poste";
        first = false;
    }
    if (salaire != 0) {
        if (!first) update += ", ";
        update += "SALAIRE = :salaire";
    }

    update += " WHERE ID_EMPLOYE = :id";

    query.prepare(update);
    query.bindValue(":id", id);

    if (!nom.isEmpty()) query.bindValue(":nom", nom);
    if (!prenom.isEmpty()) query.bindValue(":prenom", prenom);
    if (date.isValid() && !date.isNull()) query.bindValue(":date", date);
    if (!poste.isEmpty()) query.bindValue(":poste", poste);
    if (salaire != 0) query.bindValue(":salaire", salaire);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Error updating employee:" << query.lastError().text();
    }
    return success;
}








