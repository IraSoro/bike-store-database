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

    if (LineEditLogin == "" || LineEditParol == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }

    QSqlQuery query;

    if (query.exec("SELECT login_klienta, parol_klienta FROM Klient WHERE login_klienta = \'" +
                   LineEditLogin + "\' AND parol_klienta = \'" + LineEditParol + "\'")){

        if (query.next()){
            KlientLogin = query.value(0).toString();
            KlientParol = query.value(1).toString();
            //ui->textEdit->insertPlainText(KlientLogin + " " + KlientParol + "\n");

        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Введенные данные неверные.\nПроверьте введенные данные или воспользуйтесь регистрацией.");
            msgBox.exec();
        }
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    QString LineEditLogin = ui->lineEdit->text();
    QString LineEditParol = ui->lineEdit_2->text();
    QString LineEditFIO = ui->lineEdit_3->text();

    if (LineEditLogin == "" || LineEditParol == "" || LineEditFIO == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }

    QSqlQuery query;

    if (query.exec("SELECT login_klienta, parol_klienta FROM Klient WHERE login_klienta = \'" +
                   LineEditLogin + "\'")){

        if (query.next()){
            QMessageBox msgBox;
            msgBox.setText("Данный логин уже существует.\nПридумайте другой.");
            msgBox.exec();

        }
        else {
            query.exec("INSERT INTO Klient (login_klienta, parol_klienta, fio_klienta) "
                           "VALUES (\'" + LineEditLogin + "\', \'" + LineEditParol + "\', \'" + LineEditFIO + "\')");

//                query.exec("SELECT login_klienta, parol_klienta, fio_klienta FROM Klient");

//                while (query.next())
//                {
//                    QString _id = query.value(0).toString();
//                    QString name = query.value(1).toString();
//                    QString age = query.value(2).toString();
//                    ui->textEdit->insertPlainText(_id+" "+name+" "+age+"\n");
//                }
        }
    }

}
