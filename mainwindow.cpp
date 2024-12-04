#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QIcon>
#include <QMessageBox>
#include <cstddef>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"dialog.h"




#include <QDialog>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  // Call this only once


    connect(ui->changePasswordButton, &QPushButton::clicked, this, &MainWindow::on_changePasswordButton_clicked);
    connect(ui->forgotPasswordButton, &QPushButton::clicked, this, &MainWindow::on_forgotPasswordButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButtom_clicked()
{
    QString username = ui->Username->text();
    QString password = ui->Password->text();

    if (username == "ADMIN" && password == currentPassword) {
        QMessageBox::information(this, "Login Success", "Welcome, ADMIN!");
        qDebug() << "Creating dialog...";
        dialog = new Dialog(this);
        if (dialog) {
            qDebug() << "Dialog created successfully!";
            dialog->show();
        } else {
            qDebug() << "Dialog creation failed!";
        }
    } else {
        QMessageBox::warning(this, "Login Error", "Incorrect username or password!");
    }
}



void MainWindow::on_changePasswordButton_clicked()
{
    bool ok;
    QString oldPassword = QInputDialog::getText(this, "Change Password",
                                                "Enter current password:", QLineEdit::Password, "", &ok);

    if (ok && oldPassword == currentPassword) {
        QString newPassword = QInputDialog::getText(this, "Change Password",
                                                    "Enter new password:", QLineEdit::Password, "", &ok);
        if (ok && !newPassword.isEmpty()) {
            currentPassword = newPassword;
            QMessageBox::information(this, "Success", "Password changed successfully!");
        } else {
            QMessageBox::warning(this, "Error", "Password cannot be empty!");
        }
    } else {
        QMessageBox::warning(this, "Error", "Incorrect current password!");
    }
}

void MainWindow::on_forgotPasswordButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reset Password",
                                  "Are you sure you want to reset your password?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        currentPassword = "ADMIN";  // Reset to default or any recovery logic
        QMessageBox::information(this, "Password Reset", "Your password has been reset to 'ADMIN'.");
    }
}
