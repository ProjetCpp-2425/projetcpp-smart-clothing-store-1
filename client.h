
#ifndef CLIENT_H
#define CLIENT_H
#include <QString>
#include <QSqlQueryModel>
#include<QTableView>


class client
{
public:
    client();
    client(int,QString ,QString ,QString , QString ,int);
    int getIDC();
    QString getNOM();
    QString getPRENOM();
    QString getEMAIL();
    QString getADRESSE();
    int getTELEPHONE();

    void setIDC(int);
    void setNOM(QString);
    void setPRENOM(QString);
    void setEMAIL(QString);
    void setADRESSE(QString);
    void setTELEPHONE(int);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool Modifier(int,QString,QString ,QString,QString,int);
    void rechercher(const QString &a, QTableView *g);
    //QSqlQueryModel* trierclient();
    QSqlQueryModel* trierclient(const QString& critere);
    bool remise(int IDC);
    /**/ /* int nbrevent(int); */
    bool exporterPDF();
    int calculerPoints(int IDC);
    std::map<QString,int> statNbrPerType();





private:
    int IDC , TELEPHONE;
    QString NOM,PRENOM,EMAIL,ADRESSE;

};

#endif // CLIENT_H


























