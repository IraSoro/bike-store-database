#include "windowpredpriatia.h"
#include "ui_windowpredpriatia.h"

WindowPredpriatia::WindowPredpriatia(int idPred, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPredpriatia)
{
    ui->setupUi(this);
    setWindowTitle("Предприятие");

    IdPred = idPred;

    ui->dateTimeEdit_Start->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_Finish->setDateTime(QDateTime::currentDateTime().addYears(2));

}

WindowPredpriatia::~WindowPredpriatia()
{
    delete ui;
}

void WindowPredpriatia::on_pushButton_add_post_clicked()
{
    QString LineEditName = ui->lineEdit_NamePos->text();
    QString LineEditKod = ui->lineEdit_KodPos->text();
    QString LineEditDir = ui->lineEdit_FIO_Dir->text();
    QString LineEditBuh = ui->lineEdit_FIO_B->text();
    QString LineEditChet = ui->lineEdit_Chet->text();
    QString LineEditLogin = ui->lineEdit_Login->text();
    QString LineEditParol = ui->lineEdit_Parol->text();
    QDateTime DateTimeEditStart = ui->dateTimeEdit_Start->dateTime();
    QDateTime DateTimeEditFinish = ui->dateTimeEdit_Finish->dateTime();

    qDebug()<<"Start = "<<DateTimeEditStart<< "Finish = "<<DateTimeEditFinish;


    if (LineEditName == "" || LineEditKod == "" || LineEditDir == "" || LineEditBuh == "" || LineEditChet == ""
                           || LineEditLogin == "" || LineEditParol == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }

    QSqlQuery query, queryDoc, queryPos;

    if(query.exec("INSERT INTO Postavsik (kod_postavsika, nazvanie, foi_directora_postavsika, foi_glavnogo_buhgaltera_postavsika,"
               "nomer_cheta_postavsika, login_postavsika, parol_postavsika) "
               "VALUES (\'" + LineEditKod + "\', \'" + LineEditName + "\', \'" + LineEditDir + "\', \'" +
               LineEditBuh + "\', \'"+ LineEditChet + "\', \'" + LineEditLogin + "\', \'" + LineEditParol + "\')")){

    }

    queryPos.exec("SELECT id_postavsika FROM Postavsik WHERE kod_postavsika = \'" + LineEditKod+ "\'");
    if (queryPos.first())
        qDebug()<<queryPos.value(0).toString();

    queryDoc.exec("UPDATE Dogovor_s_Postavsikom SET data_nachala = \'" + DateTimeEditStart.toString(Qt::ISODateWithMs) +
                  "\' , data_okonchania = \'" + DateTimeEditFinish.toString(Qt::ISODateWithMs) + "\'WHERE id_postavsika = "+
                  queryPos.value(0).toString());




//    this->close();
//    emit firstWindow();

}
