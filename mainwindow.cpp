#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/User/Desktop/База_Данных/BD_Velosiped.db");
    db.open();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString LineEditLogin = ui->lineEdit->text();
    QString LineEditParol = ui->lineEdit_2->text();

    QSqlQuery query;

    if (query.exec("SELECT login_klienta, parol_klienta FROM Klient WHERE login_klienta = \'" +
                   LineEditLogin + "\' AND parol_klienta = \'" + LineEditParol + "\'")){

        if (query.next()){
            login = query.value(0).toString();
            parol = query.value(1).toString();
            ui->textEdit->insertPlainText(login + " " + parol + "\n");

        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Введенные данные неверные.\nПроверьте введенные данные или воспользуйтесь регистрацией.");
            msgBox.exec();
        }
    }

}
