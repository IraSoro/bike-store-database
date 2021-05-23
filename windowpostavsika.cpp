#include "windowpostavsika.h"
#include "ui_windowpostavsika.h"

//TODO: проверка поступления новых заказов, выполнение заказов

WindowPostavsika::WindowPostavsika(int idPostavsika, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPostavsika)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет поставщика");
    if (ui->tabWidget->currentIndex() == 1){

    }

    IdPostavsika = idPostavsika;

    UpdateForm();


}

WindowPostavsika::~WindowPostavsika()
{
    delete ui;
}

void WindowPostavsika::on_pushButton_Add_clicked()
{
    QString LineEditName = ui->lineEdit_Name->text();
    QString LineEditPrice = ui->lineEdit_Price->text();
    QString LineEditPic = ui->lineEdit_Pic->text();
    QString ComboBoxCategory = ui->comboBoxC->currentText();

    if (LineEditName == "" || LineEditPrice == "" /*|| LineEditPic == ""*/){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }


    QSqlQuery queryPos, queryCompl;

    queryPos.exec("SELECT * FROM Postavsik WHERE id_postavsika = " + QString::number(IdPostavsika));
    queryPos.first();

    QString KodCompl = "К - "+QDateTime::currentDateTime().toString(Qt::ISODate);

    queryCompl.exec("INSERT INTO PostavlyaemoeComplect (id_postavsika, kod_complect, kod_postavsika,"
                    "naimenovanie, kategoria, cena) "
                    "VALUES (" + QString::number(IdPostavsika) + ", \'"+ KodCompl + "\',\'" +
                    queryPos.value("kod_postavsika").toString()+"\', \'" +LineEditName + "\', \'" +
                    ComboBoxCategory + "\'," + LineEditPrice  +")" );

    QMessageBox msgBox;
    msgBox.setText("Введенное комплектующее успешно добавлено");
    msgBox.exec();
    return;



}

void WindowPostavsika::on_pushButton_Open_clicked()
{
    FileName = QFileDialog::getOpenFileName(this,tr("Select video"),"C:/Users/User/Desktop/bd",tr("Video files (*.jpg)"));
    ui->lineEdit_Pic->setText(FileName);
}

void WindowPostavsika::on_pushButton_Show_clicked() //показать заказы
{
    row = 0;

    ui->pushButton_Execute->setEnabled(true);

    while (ui->tableWidget_OrderPost->rowCount() > 0){
        ui->tableWidget_OrderPost->removeRow(0);
    }

    QString kodOrder = ui->listWidget_ShowOrder->currentIndex().data().toString();
    if (kodOrder == ""){
        QMessageBox msgBox;
        msgBox.setText("Заказ не выбран.");
        msgBox.exec();
        return;
    }

    QSqlQuery queryOrder;

    if (queryOrder.exec("SELECT * FROM Zakaz_complect WHERE oplata = 1 AND prinat = 0 AND kod_zakaza = \'" +kodOrder+"\'")){
        ui->tableWidget_OrderPost->setColumnCount(3);
        QStringList name_table;
        name_table << "Категория" << "Комплектующее" << "Количество";
        ui->tableWidget_OrderPost->setHorizontalHeaderLabels(name_table);

        while (queryOrder.next()){
            ui->tableWidget_OrderPost->insertRow( ui->tableWidget_OrderPost->rowCount() );

            QSqlQuery queryCompl;
            if (queryCompl.exec("SELECT * FROM PostavlyaemoeComplect WHERE kod_complect = \'" + queryOrder.value("kod_complect").toString()+"\'")){
                queryCompl.first();
                QTableWidgetItem *itm = new QTableWidgetItem(queryCompl.value("kategoria").toString());
                ui->tableWidget_OrderPost->setItem(row,0,itm);
                QTableWidgetItem *itm1 = new QTableWidgetItem(queryCompl.value("naimenovanie").toString());
                ui->tableWidget_OrderPost->setItem(row,1,itm1);
                QTableWidgetItem *itm2 = new QTableWidgetItem(queryOrder.value("kolichestvo").toString());
                ui->tableWidget_OrderPost->setItem(row,2,itm2);

                ui->tableWidget_OrderPost->update();
                row++;
            }
        }
    }


}

void WindowPostavsika::on_pushButton_Execute_clicked()
{
    if (row == 0){
        QMessageBox msgBox;
        msgBox.setText("Заказ пустой.");
        msgBox.exec();
        return;
    }

    QString kodOrder = ui->listWidget_ShowOrder->currentIndex().data().toString();

    QSqlQuery queryOrder, queryWarehouse, queryCompl;

    QString TitleModel = "<br>";
    QString DateOrder, DateDelivery;

    if (queryOrder.exec("SELECT * FROM Zakaz_complect WHERE oplata = 1 AND prinat = 0 AND kod_zakaza = \'" +kodOrder+"\'")){

        while (queryOrder.next()){
            DateOrder = queryOrder.value("data_zakaza").toString();
            DateDelivery = queryOrder.value("data_dostavki").toString();
            if (queryCompl.exec("SELECT * FROM PostavlyaemoeComplect WHERE kod_complect = \'" + queryOrder.value("kod_complect").toString()+"\'")){
                queryCompl.first();
                if (queryWarehouse.exec("SELECT * FROM Sklad_Complect")){
                    TitleModel += queryCompl.value("naimenovanie").toString() + "<br>";
                    bool found = false;
                    while (queryWarehouse.next()){
                        if (queryWarehouse.value("id_complect").toString() == queryOrder.value("id_complect").toString()){
                            found = true;
                        }else{
                            QSqlQuery queryUpdateCountCompl;
                            int count = queryWarehouse.value("kolichestvo").toInt() + queryOrder.value("kolichestvo").toInt();
                            queryUpdateCountCompl.exec("UPDATE Sklad_Complect SET kolichestvo = " + QString::number(count) + " WHERE id_complect = "+queryOrder.value("id_complect").toString());
                        }
                    }
                    if (!found){
                        QSqlQuery queryAddComplInSklad;
                        if (queryAddComplInSklad.exec("INSERT INTO Sklad_Complect (id_complect, kod_na_sklade, kod_complect, "
                                                      "naimenovanie, kolichestvo)"
                                                      "VALUES (" +queryCompl.value("id_complect").toString()+", \'KC - "+ queryCompl.value("id_complect").toString()+
                                                      "\', \'" +queryCompl.value("kod_complect").toString()+ "\', \'" + queryCompl.value("naimenovanie").toString()+
                                                      "\', "+ queryOrder.value("kolichestvo").toString()+")")){


                        }
                    }
                    QSqlQuery queryUpdateOrder;
                    queryUpdateOrder.exec("UPDATE Zakaz_complect SET prinat = 1 WHERE kod_zakaza = \'" +kodOrder+"\' AND kod_complect = \'" + queryCompl.value("kod_complect").toString()+ "\'");

                }
            }
        }
    }

    QSqlQuery queryCompany;

    if (queryCompany.exec("SELECT * FROM Predpriatie WHERE id_predpriatia = " + QString::number(IdPred))){
        queryCompany.first();
    }

    QSqlQuery queryPos;

    if (queryPos.exec("SELECT * FROM Postavsik WHERE id_postavsika = " + QString::number(IdPostavsika))){
        queryPos.first();
    }


    QString html =
    "<h1 align=center>"
    "Документ на поставку заказа<br>№ " + kodOrder+"</h1>"
    "<p align=justify>"
    "Заказчик: предприятие" + queryCompany.value("kod_predpriatia").toString()+"<br>"
    "Директор: " + queryCompany.value("fio_directora").toString()+"<br>"
    "Заказываемые модели: " + TitleModel+"<br>"
    "Поставляемая организация: " + queryPos.value("nazvanie").toString()+"<br>"
    "Директор организации: " + queryPos.value("foi_directora_postavsika").toString()+"<br>"
    "Дата оформления заказа: " + DateOrder+"<br>"
    "Дата доставки: " + DateDelivery+"<br>"
    "Заказ оплачен <br>"
    "</p>"
    "<div align=right>IS</div>";

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    QString temp = "Z-Z-122";
    QString FileName = "C:/Users/User/Desktop/bd/Payment_doc_complex_company/"+temp+".pdf";
    qDebug()<<FileName;
    printer.setOutputFileName(FileName);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    QMessageBox msgBox;
    msgBox.setText("Документ на поставку сохранен в "+temp+".pdf");
    msgBox.exec();

    while (ui->tableWidget_OrderPost->rowCount() > 0){
        ui->tableWidget_OrderPost->removeRow(0);
    }

    ui->listWidget_ShowOrder->clear();
    UpdateForm();

}

void WindowPostavsika::UpdateForm(){
    QSqlQuery queryOrder;

    int i = 0;
    if (queryOrder.exec("SELECT * FROM Zakaz_complect WHERE oplata = 1 AND prinat = 0 AND id_postavsika = " + QString::number(IdPostavsika))){
        while (queryOrder.next()){
            QString name = queryOrder.value("kod_zakaza").toString();
            QListWidgetItem * newItem = new QListWidgetItem ;

            bool found = false;
            for (int j = 0; j < ui->listWidget_ShowOrder->count(); ++j) {
                if (ui->listWidget_ShowOrder->item(j)->data(Qt::DisplayRole).toString() == name) {
                    found = true;
                    break;
                }
            }

            if (!found){
                newItem -> setText(name);
                ui->listWidget_ShowOrder-> insertItem(i , newItem);
                i++;
            }

            i++;
        }
    }

    ui->pushButton_Execute->setEnabled(false);
}
