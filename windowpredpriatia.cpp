// TODO: оформить договори при добавлении поставщиков, сделать проверку сроков договоров и при необходимости заключать (продлевать новые), изменить в договорах с поставщиками DateTime на Date
#include "windowpredpriatia.h"
#include "ui_windowpredpriatia.h"

WindowPredpriatia::WindowPredpriatia(int idPred, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPredpriatia)
{
    ui->setupUi(this);
    setWindowTitle("Предприятие");

    IdPred = idPred;

    ui->dateEdit_Start->setDate(QDate::currentDate());
    ui->dateEdit_Finish->setDate(QDate::currentDate().addYears(2));


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

    QSqlQuery queryContract;
    if (queryContract.exec("SELECT * FROM Dogovor_s_Postavsikom")){
        int i = 0;
        while (queryContract.next()){
            QListWidgetItem * newItemDate = new QListWidgetItem ;
            newItemDate -> setText(queryContract.value("kod_dogovora").toString());

            //QDate StartContract = queryContract.value("data_nachala").toDate();
            QDate FinishContract = queryContract.value("data_okonchania").toDate();
            QDate Now = QDate::currentDate();

            int days = Now.daysTo(FinishContract);

            qDebug()<<"days = "<<days;
            if (days < 0){
                ui->listWidget_Overdue-> insertItem(i , newItemDate);
            }
            else if (days < 7){
                ui->listWidget_Soon-> insertItem(i , newItemDate);
            }else{
                ui->listWidget_Good-> insertItem(i , newItemDate);
            }

            i++;
        }
    }


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
    QDate DateEditStart = ui->dateEdit_Start->date();
    QDate DateEditFinish = ui->dateEdit_Finish->date();

    qDebug()<<"Start = "<<DateEditStart<< "Finish = "<<DateEditFinish;


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

    queryDoc.exec("UPDATE Dogovor_s_Postavsikom SET data_nachala = \'" + DateEditStart.toString(Qt::ISODateWithMs) +
                  "\' , data_okonchania = \'" + DateEditFinish.toString(Qt::ISODateWithMs) + "\'WHERE id_postavsika = "+
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
    while (ui->tableWidget->rowCount() > 0){
        ui->tableWidget->removeRow(0);
    }

    if (ui->listWidget_PostInBasket->currentIndex().data().toString() == ""){
        QMessageBox msgBox;
        msgBox.setText("Поставщик не выбран.");
        msgBox.exec();
        return;
    }

    ui->pushButton_Pay->setEnabled(true);

    QString SelectedPost = ui->listWidget_PostInBasket->currentIndex().data().toString();


    QSqlQuery querySearchPost;

    if (querySearchPost.exec("SELECT id_postavsika FROM Postavsik WHERE nazvanie = \'" + SelectedPost + "\'")){
        if (querySearchPost.first()){
            IdSelectedPostInBasket = querySearchPost.value(0).toString();
            qDebug()<<"id = "<<IdSelectedPostInBasket;
        }
    }

    QSqlQuery queryCompl;

    QString NameModel = "";
    QSqlQuery queryZakazCompl;
    if (queryZakazCompl.exec("SELECT * FROM Zakaz_complect WHERE oplata = 0 AND id_postavsika = " + IdSelectedPostInBasket)){

        ui->tableWidget->setColumnCount(3);
        QStringList name_table;
        name_table << "Модель" << "Количество" << "Цена";
        ui->tableWidget->setHorizontalHeaderLabels(name_table);

        int i = 0;

        while (queryZakazCompl.next()){
            ui->tableWidget->insertRow( ui->tableWidget->rowCount() );

            QString IdModel = queryZakazCompl.value("id_complect").toString();

            if (queryCompl.exec("SELECT naimenovanie FROM PostavlyaemoeComplect WHERE id_complect = " + IdModel)){
                if (queryCompl.first()){
                    NameModel = queryCompl.value(0).toString();
                    TitleModel += (NameModel + "<br>");
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

void WindowPredpriatia::on_pushButton_Pay_clicked()
{

    QSqlQuery queryZakazCompl, queryUpdatePay;
    if (queryZakazCompl.exec("SELECT * FROM Zakaz_complect WHERE oplata = 0 AND id_postavsika = " + IdSelectedPostInBasket)){
        QString kodZakaza = "ЗК - " + QDateTime::currentDateTime().toString(Qt::ISODate);
        QString StartDate = QDate::currentDate().toString(Qt::ISODate);
        QString FinishDate = QDate::currentDate().addDays(30).toString(Qt::ISODate);
        qDebug()<<"start = "<<StartDate<<"   finish = "<<FinishDate;
        while (queryZakazCompl.next()){

            if(queryUpdatePay.exec("UPDATE Zakaz_complect SET oplata = 1, kod_zakaza = \'"+kodZakaza+
                                "\', data_zakaza = \'"+StartDate+"\', data_dostavki = \'" + FinishDate + "\' "
                                   "WHERE oplata = 0 AND id_postavsika = " + IdSelectedPostInBasket)){

            }
        }

        QSqlQuery queryCompany;

        if (queryCompany.exec("SELECT * FROM Predpriatie WHERE id_predpriatia = " + QString::number(IdPred))){
            queryCompany.first();
        }

        QSqlQuery queryPos;

        if (queryPos.exec("SELECT * FROM Postavsik WHERE id_postavsika = " + IdSelectedPostInBasket)){
            queryPos.first();
        }

        QString html =
        "<h1 align=center>"
        "Документ об оплате заказа<br>№ " + kodZakaza+"</h1>"
        "<p align=justify>"
        "Заказчик: предприятие" + queryCompany.value("kod_predpriatia").toString()+"<br>"
        "Директор: " + queryCompany.value("fio_directora").toString()+"<br>"
        "Заказываемые модели: " + TitleModel+"<br>"
        "Поставляемая организация: " + queryPos.value("nazvanie").toString()+"<br>"
        "Директор организации: " + queryPos.value("foi_directora_postavsika").toString()+"<br>"
        "Дата оформления заказа: " + StartDate+"<br>"
        "Дата доставки: " + FinishDate+"<br>"
        "Оплачено: " + QString::number(sum)+" рублей <br>"
        "</p>"
        "<div align=right>IS</div>";

        QTextDocument document;
        document.setHtml(html);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        QString temp = "Z - 122";
        QString FileName = "C:/Users/User/Desktop/bd/Payment_doc_complex_company/"+temp+".pdf";
        qDebug()<<FileName;
        printer.setOutputFileName(FileName);
        printer.setPageMargins(QMarginsF(15, 15, 15, 15));

        document.print(&printer);
    }

    QMessageBox msgBox;
    msgBox.setText("Заказ оплачен.\nДокумент об оплате заказа сохранен.");
    msgBox.exec();

    while (ui->tableWidget->rowCount() > 0){
        ui->tableWidget->removeRow(0);
    }

    ui->pushButton_Pay->setEnabled(false);

}
