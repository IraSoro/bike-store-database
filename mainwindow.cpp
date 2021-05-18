#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Добро пожаловать");

    bd = QSqlDatabase::addDatabase("QSQLITE");
    bd.setDatabaseName("C:/Users/User/Desktop/bd/BD_Velosiped.db");

    if(bd.open()){
            qDebug()<<"Base open";
        } else {
            qDebug()<<"Error open base";
        }
}

MainWindow::~MainWindow()
{
    bd.close();
    delete ui;
}


void MainWindow::on_pushButton_clicked() //вход клиента
{
    QString LineEditLogin = ui->lineEdit->text();
    QString LineEditParol = ui->lineEdit_2->text();

    LineEditLogin = '2';
    LineEditParol = '2';

    if (LineEditLogin == "" || LineEditParol == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }

    QSqlQuery query(bd);

    if (query.exec("SELECT login_klienta, parol_klienta, id_klienta FROM Klient WHERE login_klienta = \'" +
                   LineEditLogin + "\' AND parol_klienta = \'" + LineEditParol + "\'")){

        if (query.next()){
            KlientLogin = query.value(0).toString();
            KlientParol = query.value(1).toString();
            IdKlienta =  query.value(2).toInt();

            qDebug()<<"IdKlienta = "<<IdKlienta;

            WindowKlient = new MainWindowKlient(IdKlienta);
            connect(WindowKlient, &MainWindowKlient::firstWindow, this, &MainWindow::show);
            WindowKlient->show();
            this->close();

        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Введенные данные неверные.\nПроверьте введенные данные или воспользуйтесь регистрацией.");
            msgBox.exec();
        }
    } else {
        //qDebug() << query.lastError().text() << " " << query.lastError().number();
    }

}

void MainWindow::on_pushButton_2_clicked() //регистрация клиента
{
    WindowRegKlient = new MainWindowRegKlient();
    connect(WindowRegKlient, &MainWindowRegKlient::firstWindow, this, &MainWindow::show);
    WindowRegKlient->show();
    this->close();
}

void MainWindow::on_pushButton_3_clicked() //вход поставщика
{
    QString LineEditLogin = ui->lineEdit_3->text();
    QString LineEditParol = ui->lineEdit_4->text();

    LineEditLogin = "222";
    LineEditParol = "222";

    if (LineEditLogin == "" || LineEditParol == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }

    QSqlQuery query(bd);

    if (query.exec("SELECT login_postavsika, parol_postavsika, id_postavsika FROM Postavsik WHERE login_postavsika = \'" +
                   LineEditLogin + "\' AND parol_postavsika = \'" + LineEditParol + "\'")){

        if (query.next()){
            KlientLogin = query.value(0).toString();
            KlientParol = query.value(1).toString();
            IdPostavsika =  query.value(2).toInt();

            qDebug()<<"IdPostavsika = "<<IdPostavsika;

            WinPostavsika = new WindowPostavsika(IdPostavsika);
            connect(WinPostavsika, &WindowPostavsika::firstWindow, this, &WindowPostavsika::show);
            WinPostavsika->show();
            this->close();

        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Введенные данные неверные.\nПроверьте введенные данные.");
            msgBox.exec();
        }
    } else {
        //qDebug() << query.lastError().text() << " " << query.lastError().number();
    }

}
