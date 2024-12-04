#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QDate>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <cstddef>
using namespace std;
class employe
{
    int id;
    QString prenom;
    QString nom;
    QDate date = QDate::currentDate();
    QString poste;
    int salaire;

public:
    // contructeurs
    employe();
    employe(int,QString,QString,QDate,QString,int);

    //Getters
    QString getNom(){return nom;}
    QString getPrenom(){return prenom;}
   QDate getDate(){return date;}
    QString getPoste(){return poste;}
    int getSalaire(){return salaire;}


    //setters
    void setNom(QString n){nom=n;}
    void setPrenom(QString p){prenom=p;}
    void setDate(QDate d){date=d;}
    void setPoste(QString pos){poste=pos;}
    void setSalaire(int s){salaire=s;}


    //fonct
    bool ajouter();
    bool supprimer(int id);
    bool modifier(int id, QString nom, QString prenom, QDate date, QString poste, int salaire) ;


    QSqlQueryModel * afficher();







};

#endif // EMPLOYE_H
