#include "windowregklient.h"
#include "ui_windowregklient.h"

MainWindowRegKlient::MainWindowRegKlient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowRegKlient)
{
    ui->setupUi(this);
    setWindowTitle("Регистрация");
}

MainWindowRegKlient::~MainWindowRegKlient()
{
    delete ui;
}

void MainWindowRegKlient::on_pushButton_clicked()
{
    QString LineEditFIO = ui->lineEdit->text();
    QString LineEditLogin = ui->lineEdit_2->text();
    QString LineEditParol = ui->lineEdit_3->text();


    if (LineEditLogin == "" || LineEditParol == "" || LineEditFIO == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }

    if (ui->checkBox->isChecked() == false){
        QMessageBox msgBox;
        msgBox.setText("Условия соглашения не приняты.");
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

            this->close();
            emit firstWindow();

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
