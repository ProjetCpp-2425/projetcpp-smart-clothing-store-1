#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QString>
#include"dialog.h"
#include <cstddef>
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
    void on_loginButtom_clicked();
    void on_changePasswordButton_clicked();
    void on_forgotPasswordButton_clicked();

private:
    Ui::MainWindow *ui;
    QString currentPassword = "ADMIN"; // Default password
    Dialog *dialog;
};

#endif // MAINWINDOW_H
