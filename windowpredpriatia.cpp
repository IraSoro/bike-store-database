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
            QListWidgetItem * newItem1 = new QListWidgetItem ;
            newItem -> setText(query.value(0).toString());
            newItem1 -> setText(query.value(0).toString());
            ui->listWidget_Post-> insertItem(i , newItem);
            ui->listWidget_PostInBasket-> insertItem(i , newItem1);
            i++;
        }
    }
    ui->pushButton_ShowModel->setEnabled(false);
    ui->pushButton_AddCount->setEnabled(false);
    ui->spinBox->setValue(10);


    ui->pushButton_Pay->setEnabled(false);

    ui->tableWidget->setColumnCount(3);
    QStringList name_table;
    name_table << "Модель" << "Количество" << "Цена";
    ui->tableWidget->setHorizontalHeaderLabels(name_table);

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
       if (query.exec("SELECT naimenovanie FROM PostavlyaemoeComplect WHERE kategoria = \'" + SelectedKat +"\' AND id_postavsika = " + QString::number(IdSelectedPost))){
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


    QSqlQuery queryPos, queryCompl, queryZakaz;

    queryCompl.exec("SELECT * FROM PostavlyaemoeComplect WHERE naimenovanie = \'" + ui->listWidget_Model->currentIndex().data().toString() + "\'");
    queryCompl.first();

    QString SelectCompl = queryCompl.value("id_complect").toString();
    qDebug()<<"------"<<SelectCompl;

    queryPos.exec("SELECT * FROM Postavsik WHERE id_postavsika = " + QString::number(IdSelectedPost));
    queryPos.first();
    qDebug()<<"------"<<queryPos.value("id_postavsika").toString();

    QString kodZakaza = "000";

    if (queryZakaz.exec("INSERT INTO Zakaz_complect (id_complect, id_postavsika, kod_zakaza, kod_postavsika, "
                    "kod_complect, kolichestvo, cena) "
                    "VALUES (" + SelectCompl+ ", "+ queryPos.value("id_postavsika").toString()+
                    ",\'"+kodZakaza+ "\', \'" + queryPos.value("kod_postavsika").toString() + "\', \'"+
                    queryCompl.value("kod_complect").toString() + "\', " + QString::number(ui->spinBox->value()) + ", "+
                    queryCompl.value("cena").toString() + ")"))
    {
        QMessageBox msgBox;
        msgBox.setText("Комплектующее добавлено в корзину.");
        msgBox.exec();
        ui->pushButton_ShowModel->setEnabled(false);
        ui->pushButton_AddCount->setEnabled(false);
        ui->listWidget_Model->clear();
        ui->listWidget_Kat->clear();

    }else {
        //qDebug()<<"no";
    }
}

void WindowPredpriatia::on_pushButton_ShowBasket_clicked()
{
    ui->tableWidget->clear();

    if (ui->listWidget_PostInBasket->currentIndex().data().toString() == ""){
        QMessageBox msgBox;
        msgBox.setText("Поставщик не выбран.");
        msgBox.exec();
        return;
    }

    ui->pushButton_Pay->setEnabled(true);

    QString SelectedPost = ui->listWidget_PostInBasket->currentIndex().data().toString();
    QString IdSelectedPostInBasket = "";

    QSqlQuery querySearchPost;

    if (querySearchPost.exec("SELECT id_postavsika FROM Postavsik WHERE nazvanie = \'" + SelectedPost + "\'")){
        if (querySearchPost.first()){
            IdSelectedPostInBasket = querySearchPost.value(0).toString();
            qDebug()<<"id = "<<IdSelectedPostInBasket;
        }
    }

    QSqlQuery queryCompl;


    int sum = 0;
    QString NameModel = "";
    QSqlQuery queryZakazCompl;
    if (queryZakazCompl.exec("SELECT * FROM Zakaz_complect WHERE oplata = 0 AND id_postavsika = " + IdSelectedPostInBasket)){

        int i = 0;
//        while(queryZakazCompl.next()){
//            i++;
//        }

        ui->tableWidget->setRowCount(10);
        ui->tableWidget->setColumnCount(3);
        QStringList name_table;
        name_table << "Модель" << "Количество" << "Цена";
        ui->tableWidget->setHorizontalHeaderLabels(name_table);
        i = 0;
        while (queryZakazCompl.next()){
            ui->tableWidget->insertRow( ui->tableWidget->rowCount() );

            QString IdModel = queryZakazCompl.value("id_complect").toString();
            qDebug()<<"id model = "<<IdModel;
            if (queryCompl.exec("SELECT naimenovanie FROM Postavsik WHERE id_complect = " + IdModel)){
                if (queryCompl.first()){
                    NameModel = queryCompl.value(0).toString();
                }
            }

            QTableWidgetItem *itm = new QTableWidgetItem(NameModel);
            ui->tableWidget->setItem(i,0,itm);
            QTableWidgetItem *itm1 = new QTableWidgetItem(queryZakazCompl.value("kolichestvo").toString());
            ui->tableWidget->setItem(i,1,itm1);
            sum += queryZakazCompl.value("cena").toInt();
            QTableWidgetItem *itm2 = new QTableWidgetItem(queryZakazCompl.value("cena").toString());
            ui->tableWidget->setItem(i,2,itm2);
            ui->tableWidget->update();
            i++;

        }
    }

    ui->lineEdit_Total->setText(QString::number(sum));


}
