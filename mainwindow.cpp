#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Добро пожаловать");

    bd = QSqlDatabase::addDatabase("QSQLITE");
    bd.setDatabaseName("C:/Users/User/Desktop/bd/BD_Velosiped.db");
    bd.open();
}

MainWindow::~MainWindow()
{
    bd.close();
    delete ui;
}


void MainWindow::on_pushButton_clicked() //вход
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

            WindowKlient = new MainWindowKlient();
            connect(WindowKlient, &MainWindowKlient::firstWindow, this, &MainWindow::show);
            WindowKlient->show();
            this->close();

        }
        else {
            QMessageBox msgBox;
            msgBox.setText("Введенные данные неверные.\nПроверьте введенные данные или воспользуйтесь регистрацией.");
            msgBox.exec();
        }
    }
}

void MainWindow::on_pushButton_2_clicked() //регистрация
{
    WindowRegKlient = new MainWindowRegKlient();
    connect(WindowRegKlient, &MainWindowRegKlient::firstWindow, this, &MainWindow::show);
    WindowRegKlient->show();
    this->close();
}
