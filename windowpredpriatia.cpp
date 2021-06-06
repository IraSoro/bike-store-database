// TODO: проверить код поставщика и во всех таблицах (он разный!!!), проверить итоговую цену (возможно она не умножается на количество товаров)
#include "windowpredpriatia.h"
#include "ui_windowpredpriatia.h"

WindowPredpriatia::WindowPredpriatia(int idPred, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPredpriatia)
{
    ui->setupUi(this);
    setWindowTitle("Предприятие");

    IdPred = idPred;

    UpdateForm();
}

WindowPredpriatia::~WindowPredpriatia()
{
    delete ui;
}


void WindowPredpriatia::UpdateForm(){
    ui->dateEdit_Start->setDate(QDate::currentDate());
    ui->dateEdit_Finish->setDate(QDate::currentDate().addYears(2));

    ui->listWidget_Kat->clear();
    ui->listWidget_Model->clear();
    ui->listWidget_Good->clear();
    ui->listWidget_Post->clear();
    ui->listWidget_Soon->clear();
    ui->listWidget_Overdue->clear();
    ui->listWidget_PostInBasket->clear();

    QSqlQuery query, querySearchContract;

    if (query.exec("SELECT * FROM Postavsik")){
        int i = 0;
        while (query.next()){
            QSqlQuery querySearchContract;
            if (querySearchContract.exec("SELECT * FROM Dogovor_s_Postavsikom "
                                         "WHERE prodlen = 0 AND id_postavsika = "+
                                         query.value("id_postavsika").toString())){

                querySearchContract.first();
                QDate Now = QDate::currentDate();
                QDate FinishDate = querySearchContract.value("data_okonchania").toDate();
                int days = Now.daysTo(FinishDate);

                if (days > 0){
                    QString name = query.value("nazvanie").toString();
                    QListWidgetItem * newItem = new QListWidgetItem ;
                    QListWidgetItem * newItem1 = new QListWidgetItem ;
                    newItem -> setText(name);
                    newItem1 -> setText(name);
                    ui->listWidget_Post-> insertItem(i , newItem);
                    ui->listWidget_PostInBasket-> insertItem(i , newItem1);
                    i++;
                }

            }

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

            bool prolong = queryContract.value("prodlen").toBool();

            //qDebug()<<"days = "<<days;
            if (days < 0 && prolong == 0){
                ui->listWidget_Overdue-> insertItem(i , newItemDate);
            }
            else if (days < 7 && prolong == 0){
                ui->listWidget_Soon-> insertItem(i , newItemDate);
            }else {
                ui->listWidget_Good-> insertItem(i , newItemDate);
            }

            i++;
        }
    }


    int priv = 0, uprav = 0, ped = 0, torm = 0, kol = 0, rez = 0, tros = 0, ram = 0;
    QSqlQuery queryBiuldBike;
    if (queryBiuldBike.exec("SELECT * FROM Sklad_Complect")){
        while (queryBiuldBike.next()){
            QString View = "";
            QString Model = queryBiuldBike.value("naimenovanie").toString();
            QSqlQuery querySearchView;
            if (querySearchView.exec("SELECT kategoria FROM PostavlyaemoeComplect WHERE id_complect = " + queryBiuldBike.value("id_complect").toString())){
                querySearchView.first();
                View = querySearchView.value(0).toString();
                QListWidgetItem * newItem = new QListWidgetItem ;
                newItem -> setText(Model);
                if (View == "Привод"){
                    ui->listWidget_Privod->insertItem(priv , newItem);
                    priv++;
                } else
                if (View == "Управление"){
                    ui->listWidget_Upravl->insertItem(uprav, newItem);
                    uprav++;
                } else
                if (View == "Педали"){
                    ui->listWidget_Pedal->insertItem(ped, newItem);
                    ped++;
                } else
                if (View == "Тормоза и запчасти"){
                    ui->listWidget_Tormoz->insertItem(torm, newItem);
                    torm++;
                } else
                if (View == "Колеса"){
                    ui->listWidget_Kolesa->insertItem(kol, newItem);
                    kol++;
                } else
                if (View == "Резина"){
                    ui->listWidget_Rezina->insertItem(rez, newItem);
                    rez++;
                } else
                if (View == "Тросы и оплетки"){
                    ui->listWidget_Tros->insertItem(tros, newItem);
                    tros++;
                } else
                if (View == "Рамки, вилки, задние амортизаторы"){
                    ui->listWidget_Ramki->insertItem(ram, newItem);
                    ram++;
                }
            }
        }
    }

    ui->pushButton_AddedSchema->setEnabled(false);
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

    QSqlQuery query, queryDoc, queryPos, queryPred, queryGet;

    if(query.exec("INSERT INTO Postavsik (kod_postavsika, nazvanie, foi_directora_postavsika, foi_glavnogo_buhgaltera_postavsika,"
               "nomer_cheta_postavsika, login_postavsika, parol_postavsika) "
               "VALUES (\'" + LineEditKod + "\', \'" + LineEditName + "\', \'" + LineEditDir + "\', \'" +
               LineEditBuh + "\', \'"+ LineEditChet + "\', \'" + LineEditLogin + "\', \'" + LineEditParol + "\')")){

    }

    queryPred.exec("SELECT * FROM Predpriatie WHERE id_predpriatia = " + QString::number(IdPred));
    if (queryPred.first()){
        //qDebug()<<queryPred.value("kod_predpriatia").toString();
    }

    queryPos.exec("SELECT * FROM Postavsik WHERE kod_postavsika = \'" + LineEditKod+ "\'");
    if (queryPos.first()){
        //qDebug()<<queryPos.value(0).toString();
    }

    queryDoc.exec("UPDATE Dogovor_s_Postavsikom SET data_nachala = \'" + DateEditStart.toString(Qt::ISODateWithMs) +
                  "\' , data_okonchania = \'" + DateEditFinish.toString(Qt::ISODateWithMs) + "\'WHERE id_postavsika = "+
                  queryPos.value("id_postavsika").toString());

    queryGet.exec("SELECT * FROM Dogovor_s_Postavsikom WHERE id_postavsika = " + queryPos.value("id_postavsika").toString());
    if (queryGet.first()){
        //qDebug()<<queryGet.value(0).toString();
    }


    QString html =
    "<h1 align=center>"
    "Договор с поставщиком<br>№ " + queryGet.value("kod_dogovora").toString()+"</h1>"
    "<p align=justify>"
    "Предприятие: " + queryPred.value("kod_predpriatia").toString()+"<br>"
    "Директор: " + queryPred.value("fio_directora").toString()+"<br>"
    "Поставляемая организация: " + queryPos.value("nazvanie").toString()+"<br>"
    "Директор организации: " + queryPos.value("foi_directora_postavsika").toString()+"<br>"
    "Дата начала действия договора: " + DateEditStart.toString(Qt::ISODateWithMs)+"<br>"
    "Дата окончания действия договора: " + DateEditFinish.toString(Qt::ISODateWithMs) +"<br>"
    "</p>"
    "<div align=right>IS</div>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    QString temp = queryGet.value("kod_dogovora").toString();
    QString FileName = "C:/Users/User/Desktop/bd/Contract_postavsiki/"+temp+".pdf";
    qDebug()<<FileName;
    printer.setOutputFileName(FileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    QMessageBox msgBox;
    msgBox.setText("Договор № " +queryGet.value("kod_dogovora").toString()+" сохранен.");
    msgBox.exec();

    ui->lineEdit_NamePos->clear();
    ui->lineEdit_KodPos->clear();
    ui->lineEdit_FIO_Dir->clear();
    ui->lineEdit_FIO_B->clear();
    ui->lineEdit_Chet->clear();
    ui->lineEdit_Login->clear();
    ui->lineEdit_Parol->clear();

    UpdateForm();

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


void WindowPredpriatia::on_pushButton_AddCount_clicked()  //добавить в корзину
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

    QString kodZakaza = "0";

    if (queryZakaz.exec("INSERT INTO Zakaz_complect (id_complect, id_postavsika, id_zakaza, kod_zakaza, kod_postavsika, "
                    "kod_complect, kolichestvo, cena) "
                    "VALUES (" + SelectCompl+ ", "+ queryPos.value("id_postavsika").toString()+
                    ", 0, \'"+kodZakaza+ "\', \'" + queryPos.value("kod_postavsika").toString() + "\', \'"+
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
    sum = 0;

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
    if (queryZakazCompl.exec("SELECT * FROM Zakaz_complect WHERE id_zakaza = 0 AND id_postavsika = " + IdSelectedPostInBasket)){

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
            sum += queryZakazCompl.value("cena").toInt()*queryZakazCompl.value("kolichestvo").toInt();
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
    QString temp = "";
    double sumTax = 0;
    QSqlQuery queryZakazCompl, queryUpdatePay;
    if (queryZakazCompl.exec("SELECT * FROM Zakaz_complect WHERE id_zakaza = 0 AND id_postavsika = " + IdSelectedPostInBasket)){
        QString kodZakaza = "ЗК - " + QDateTime::currentDateTime().toString(Qt::ISODate);
        QString StartDate = QDate::currentDate().toString(Qt::ISODate);
        QString FinishDate = QDate::currentDate().addDays(30).toString(Qt::ISODate);
        qDebug()<<"start = "<<StartDate<<"   finish = "<<FinishDate;

        if(queryUpdatePay.exec("INSERT INTO Zakaz_postavsiku (oplachen, kod_zakaza, data_zakaza, data_dostavki, summa)"
                               "VALUES (1, \'" + kodZakaza + "\', \'" + StartDate + "\' , \'" + FinishDate + "\', " + QString::number(sum) + ")")){

        }
//        QString IdOrder = "";
        while (queryZakazCompl.next()){
//            QSqlQuery queryGetIdOrder;
//            if (queryGetIdOrder.exec("SELECT max(id_zakaza) FROM Zakaz_postavsiku")){
//                IdOrder = queryGetIdOrder.value("max(id_zakaza)").toString();
//                qDebug()<<"order = "<<IdOrder;
//            }


            QSqlQuery queryAddIdOrder;
            if (queryAddIdOrder.exec("UPDATE Zakaz_complect SET id_zakaza = (SELECT max(id_zakaza) FROM Zakaz_postavsiku), "
                                     "kod_zakaza = \'" + kodZakaza + "\' "
                                     "WHERE id_complect_v_zakaze = " + queryZakazCompl.value("id_complect_v_zakaze").toString())){

            }

            sumTax = 1.2 * sum;
            QSqlQuery querySchetPost;
            if(querySchetPost.exec("INSERT INTO Schet_postavsika (id_postavsika, summa_s_nds, id_zakaza )"
                                   "VALUES ( " + IdSelectedPostInBasket + ", " + QString::number(sumTax) + ", (SELECT max(id_zakaza) FROM Zakaz_postavsiku))")){

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

        QSqlQuery queryCompl;
        QString idCompl = "";
        if (queryCompl.exec("SELECT * FROM Zakaz_postavsiku WHERE kod_zakaza = \'" + kodZakaza +"\'")){
            while(queryCompl.next()){
                idCompl += queryCompl.value("id_zakaza").toString();
            }
        }

        QString score =
        "<h1 align=center>"
        "Счет об оплате заказа<br>№ " + kodZakaza+"</h1>"
        "<p align=justify>"
        "Заказчик: предприятие " + queryCompany.value("kod_predpriatia").toString()+"<br>"
        "Директор: " + queryCompany.value("fio_directora").toString()+"<br>"
        "Заказываемые модели: " + TitleModel+"<br>"
        "Поставляемая организация: " + queryPos.value("nazvanie").toString()+"<br>"
        "Директор организации: " + queryPos.value("foi_directora_postavsika").toString()+"<br>"
        "Дата оформления заказа: " + StartDate+"<br>"
        "Дата доставки: " + FinishDate+"<br>"
        "Сумма заказа: " + QString::number(sum)+" рублей <br>"
        "Итог (с учетом НДС): " + QString::number(sumTax)+" рублей <br>"
        "Заказ оплачен<br>"
        "</p>"
        "<div align=right>IS</div>";

        QTextDocument document;
        document.setHtml(score);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        temp = "СЧЕТ - ПР - "+idCompl;
        QString FileName = "C:/Users/User/Desktop/bd/Company_score/"+temp+".pdf";
        qDebug()<<FileName;
        printer.setOutputFileName(FileName);
        printer.setPageMargins(QMarginsF(15, 15, 15, 15));

        document.print(&printer);

        QMessageBox msgBox;
        msgBox.setText("Заказ оплачен.\nСчет об оплате заказа сохранен в " + temp + ".pdf");
        msgBox.exec();



        QString html =
        "<h1 align=center>"
        "Документ об оплате заказа<br>№ " + kodZakaza+"</h1>"
        "<p align=justify>"
        "Заказчик: предприятие " + queryCompany.value("kod_predpriatia").toString()+"<br>"
        "Директор: " + queryCompany.value("fio_directora").toString()+"<br>"
        "Заказываемые модели: " + TitleModel+"<br>"
        "Поставляемая организация: " + queryPos.value("nazvanie").toString()+"<br>"
        "Директор организации: " + queryPos.value("foi_directora_postavsika").toString()+"<br>"
        "Дата оформления заказа: " + StartDate+"<br>"
        "Дата доставки: " + FinishDate+"<br>"
        "Оплачено: " + QString::number(sumTax)+" рублей <br>"
        "</p>"
        "<div align=right>IS</div>";

        QTextDocument document1;
        document1.setHtml(html);

        QPrinter printer1(QPrinter::PrinterResolution);
        printer1.setOutputFormat(QPrinter::PdfFormat);
        printer1.setPaperSize(QPrinter::A4);
        temp = "ЗАК - "+idCompl;
        QString FileName1 = "C:/Users/User/Desktop/bd/Payment_doc_complex_company/"+temp+".pdf";
        printer1.setOutputFileName(FileName1);
        printer1.setPageMargins(QMarginsF(15, 15, 15, 15));

        document1.print(&printer1);

    }



    while (ui->tableWidget->rowCount() > 0){
        ui->tableWidget->removeRow(0);
    }

    ui->pushButton_Pay->setEnabled(false);
    ui->lineEdit_Total->clear();

}

void WindowPredpriatia::ConclusionNewContract(QDate start, QDate finish, QString Kod){
    QString kodDogovora = Kod;

    QSqlQuery queryContractPostAdd, queryContractFinish;

    if (queryContractFinish.exec("SELECT * FROM Dogovor_s_Postavsikom WHERE kod_dogovora = \'" + kodDogovora+"\'")){
        queryContractFinish.first();
        qDebug()<<"1 - ok";
    }
    QDate NewStartDate = start;
    QDate NewFinishDate = finish;

    qDebug()<<"start = "<<NewStartDate.toString(Qt::ISODate)<<" finish = "<<NewFinishDate.toString(Qt::ISODate);

    QString NewContract = kodDogovora+"-2";
    if(queryContractPostAdd.exec("INSERT INTO Dogovor_s_Postavsikom "
               "(id_postavsika, id_predpriatia, kod_dogovora, kod_postavsika,"
               "kod_predpriatia, data_nachala, data_okonchania) "
               "VALUES (" + queryContractFinish.value("id_postavsika").toString()+
               "," + queryContractFinish.value("id_predpriatia").toString()+", \'"+
               NewContract + "\' , \'" +queryContractFinish.value("kod_postavsika").toString()+ "\', \'"+
               queryContractFinish.value("kod_predpriatia").toString() + "\', \'"+
               NewStartDate.toString(Qt::ISODate) + "\' , \'" +  NewFinishDate.toString(Qt::ISODate) + "\' )")){
        qDebug()<<"2 - ok";

    }

    QSqlQuery queryDoc;

    queryDoc.exec("UPDATE Dogovor_s_Postavsikom SET prodlen = 1 "
                  "WHERE kod_dogovora = \'" + kodDogovora+"\'");

    QSqlQuery queryCompany;

    if (queryCompany.exec("SELECT * FROM Predpriatie WHERE id_predpriatia = " + QString::number(IdPred))){
        queryCompany.first();
    }

    QSqlQuery queryPos;

    if (queryPos.exec("SELECT * FROM Postavsik WHERE id_postavsika = " + queryContractFinish.value("id_postavsika").toString())){
        queryPos.first();
    }

    QString html =
    "<h1 align=center>"
    "Договор с поставщиком<br>№ " + NewContract+"</h1>"
    "<p align=justify>"
    "Заказчик: предприятие " + queryCompany.value("kod_predpriatia").toString()+"<br>"
    "Директор: " + queryCompany.value("fio_directora").toString()+"<br>"
    "Поставляемая организация: " + queryPos.value("nazvanie").toString()+"<br>"
    "Директор организации: " + queryPos.value("foi_directora_postavsika").toString()+"<br>"
    "Дата начала действия договора: " + NewStartDate.toString(Qt::ISODate)+"<br>"
    "Дата окончания действия договора: " + NewFinishDate.toString(Qt::ISODate) +"<br>"
    "</p>"
    "<div align=right>IS</div>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    QString temp = NewContract;
    QString FileName = "C:/Users/User/Desktop/bd/Contract_postavsiki/"+temp+".pdf";
    qDebug()<<FileName;
    printer.setOutputFileName(FileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    QMessageBox msgBox;
    msgBox.setText("Договор № " +NewContract+" о продлении сохранен.");
    msgBox.exec();


    ui->listWidget_Overdue->clear();
    ui->listWidget_Soon->clear();
    ui->listWidget_Good->clear();

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

            bool prolong = queryContract.value("prodlen").toBool();

            qDebug()<<"days = "<<days;
            if (days < 0 && prolong == 0){
                ui->listWidget_Overdue-> insertItem(i , newItemDate);
            }
            else if (days < 7 && prolong == 0){
                ui->listWidget_Soon-> insertItem(i , newItemDate);
            }else {
                ui->listWidget_Good-> insertItem(i , newItemDate);
            }

            i++;
        }
    }

}

void WindowPredpriatia::on_pushButton_5_clicked()  //продление договора
{

    QString kodDogovora = ui->listWidget_Soon->currentIndex().data().toString();
    if (kodDogovora == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не выбрали поставщика");
        msgBox.exec();
        return;
    }

    QSqlQuery queryContractFinish;

    if (queryContractFinish.exec("SELECT * FROM Dogovor_s_Postavsikom WHERE kod_dogovora = \'" + kodDogovora+"\'")){
        queryContractFinish.first();
    }
    QDate NewStartDate = queryContractFinish.value("data_okonchania").toDate().addDays(1);
    QDate NewFinishDate = NewStartDate.addYears(2);

    ConclusionNewContract(NewStartDate, NewFinishDate, kodDogovora);

}

void WindowPredpriatia::on_pushButton_6_clicked() //просроченные договора, заключить заново
{
    QString kodDogovora = ui->listWidget_Overdue->currentIndex().data().toString();
    if (kodDogovora == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не выбрали поставщика");
        msgBox.exec();
        return;
    }

    QDate NewDateStart = QDate::currentDate().addDays(1);
    QDate NewDateFinish = NewDateStart.addYears(2);

    ConclusionNewContract(NewDateStart, NewDateFinish, kodDogovora);

}

void WindowPredpriatia::on_action_triggered()
{
    this->close();
    emit firstWindow();
}

void WindowPredpriatia::on_listWidget_Post_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<"12345678";
}

void WindowPredpriatia::on_pushButton_Price_clicked()
{
    sum = 0;

    QVector <QString> View;
    QString Privod = ui->listWidget_Privod->currentIndex().data().toString();
    View.push_back(Privod);
    QString Upravl = ui->listWidget_Upravl->currentIndex().data().toString();
    View.push_back(Upravl);
    QString Pedal = ui->listWidget_Pedal->currentIndex().data().toString();
    View.push_back(Pedal);
    QString Tormoz = ui->listWidget_Tormoz->currentIndex().data().toString();
    View.push_back(Tormoz);
    QString Kolesa = ui->listWidget_Kolesa->currentIndex().data().toString();
    View.push_back(Kolesa);
    QString Rezina = ui->listWidget_Rezina->currentIndex().data().toString();
    View.push_back(Rezina);
    QString Tros = ui->listWidget_Tros->currentIndex().data().toString();
    View.push_back(Tros);
    QString Ramki = ui->listWidget_Ramki->currentIndex().data().toString();
    View.push_back(Ramki);

    if (Privod == "" || Upravl == "" || Pedal == "" || Tormoz == "" ||
        Kolesa == "" || Rezina == "" || Tros  == "" || Ramki  == ""){
        QMessageBox msgBox;
        msgBox.setText("Вы выбрали не все комплектующие.");
        msgBox.exec();
        return;
    }


    ui->pushButton_AddedSchema->setEnabled(true);
}

void WindowPredpriatia::on_pushButton_AddedSchema_clicked()
{
    if (ui->lineEdit_TitleModel->text() == "" || ui->lineEdit_Description->text() == "" ){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не всю информацию о модели.");
        msgBox.exec();
        return;
    }
}
