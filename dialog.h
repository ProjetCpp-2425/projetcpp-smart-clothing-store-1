#ifndef DIALOG_H
#define DIALOG_H
#include<QObject>
#include <QDialog>
#include "employe.h"
#include <QMap>
#include <cstddef>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{

    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:
    void on_pushButton_8_clicked();

    void on_pushButton_12_clicked();




    void on_mon_clicked();

    void on_exporterr_clicked();

    void on_trier_clicked();

    void on_rechercher_clicked();
    void addEvent();
    void showEvents(const QDate &date);
    void clearEvent();
    void highlightEventDates();
    void showSalaryDistribution();
    void on_seniorityAnalysis_clicked();




private:
    Ui::Dialog *ui;


    employe emp;
    QMap<QDate, QStringList> events;

};

#endif // DIALOG_H
