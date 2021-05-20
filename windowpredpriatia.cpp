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


    QSqlQuery query;

    if (query.exec("SELECT nazvanie FROM Postavsik")){
        int i = 0;
        while (query.next()){
            QListWidgetItem * newItem = new QListWidgetItem ;
            newItem -> setText(query.value(0).toString());
            ui->listWidget_Post-> insertItem(i , newItem);
            i++;
        }
    }
    ui->pushButton_ShowModel->setEnabled(false);
    ui->pushButton_AddCount->setEnabled(false);
    ui->spinBox->setValue(10);

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

void WindowPredpriatia::on_pushButton_ShowKat_clicked()
{
    ui->listWidget_Kat->clear();
    ui->listWidget_Model->clear();


    QString SelectedPost = ui->listWidget_Post->currentIndex().data().toString();
    //qDebug()<<"SelectedPost = "<<SelectedPost;

    QSqlQuery querySearchPost;

    if (querySearchPost.exec("SELECT id_postavsika FROM Postavsik WHERE nazvanie = \'" + SelectedPost + "\'")){
        if (querySearchPost.first()){
            IdSelectedPost = querySearchPost.value(0).toInt();
            //qDebug()<<"id = "<<IdSelectedPost;
        }
    }

    if (SelectedPost != ""){
       ui->pushButton_ShowModel->setEnabled(true);
       QSqlQuery query;

       int i = 0;
       if (query.exec("SELECT kategoria FROM PostavlyaemoeComplect WHERE id_postavsika = " + QString::number(IdSelectedPost))){
           while (query.next()){
               QListWidgetItem * newItem = new QListWidgetItem ;
               QString Category  = "";
               Category = query.value(0).toString();

               bool found = false;
               for (int j = 0; j < ui->listWidget_Kat->count(); ++j) {
                   if (ui->listWidget_Kat->item(j)->data(Qt::DisplayRole).toString() == Category) {
                       found = true;
                       break;
                   }
               }

               if (!found){
                   newItem -> setText(Category);
                   ui->listWidget_Kat-> insertItem(i , newItem);
                   i++;
               }
           }
       }

    }else {
        return;
    }
}

void WindowPredpriatia::on_pushButton_ShowModel_clicked()
{
    ui->listWidget_Model->clear();


    QString SelectedKat = ui->listWidget_Kat->currentIndex().data().toString();
    //qDebug()<<"SelectedPost = "<<SelectedPost;

    QSqlQuery querySearchPost;


    if (SelectedKat != ""){
       QSqlQuery query;

       int i = 0;
       if (query.exec("SELECT naimenovanie FROM PostavlyaemoeComplect WHERE kategoria = \'" + SelectedKat +"\'")){
           while (query.next()){
               QListWidgetItem * newItem = new QListWidgetItem ;
               QString Model  = "";
               Model = query.value(0).toString();
               newItem -> setText(Model);
               ui->listWidget_Model-> insertItem(i , newItem);
               i++;
           }
       }

    }else {
        return;
    }

    ui->pushButton_AddCount->setEnabled(true);
}


void WindowPredpriatia::on_pushButton_AddCount_clicked()
{
    if (ui->spinBox->value() == 0){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не ввели количество комплектующего");
        msgBox.exec();
        return;
    }

    if (ui->listWidget_Model->currentIndex().data().toString() == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не выбрали комплектующее");
        msgBox.exec();
        return;
    }

}
