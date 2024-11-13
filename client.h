
#ifndef CLIENT_H
#define CLIENT_H
#include <QString>
#include <QSqlQueryModel>
#include<QTableView>


class client
{
public:
    client();
    client(int,QString ,QString ,QString , QString );
    int getIDC();
    QString getNOM();
    QString getPRENOM();
    QString getEMAIL();
    QString getADRESSE();
    void setIDC(int);
    void setNOM(QString);
    void setPRENOM(QString);
    void setEMAIL(QString);
    void setADRESSE(QString);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool Modifier(int,QString,QString ,QString,QString);
    void rechercher(const QString &a, QTableView *g);
    //QSqlQueryModel* trierclient();
    QSqlQueryModel* trierclient(const QString& critere);
    bool exporterPDF();







private:
    int IDC;
    QString NOM,PRENOM,EMAIL,ADRESSE;

};

#endif // CLIENT_H


























