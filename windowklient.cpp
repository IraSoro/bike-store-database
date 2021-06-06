#include "windowklient.h"
#include "ui_windowklient.h"

// TODO: информация об статусе заказов

MainWindowKlient::MainWindowKlient( int idKlienta, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowKlient)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет");

    IdKlienta = idKlienta;

    QSqlQuery query;

    if (query.exec("SELECT model FROM Sklad_Velosipedov WHERE kolichestvo_na_sklade > 0")){
        int i = 0;
        while (query.next()){
            QListWidgetItem * newItem = new QListWidgetItem ;
            newItem -> setText(query.value(0).toString());
            ui->listWidget -> insertItem(i , newItem);
            i++;
        }
    }

    ui->pushButton_2->setEnabled(false);

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

    ui->pushButton_3->setEnabled(false);

    UpdateBasket();
    UpdateStatusOrder();
}

MainWindowKlient::~MainWindowKlient()
{
    delete ui;
}

void MainWindowKlient::UpdateStatusOrder(){
    QDate Now = QDate::currentDate();

    //QString DateBuilding = QDate::currentDate().addDays(20).toString(Qt::ISODateWithMs);
    //QString DateDelivery = QDate::currentDate().addDays(30).toString(Qt::ISODateWithMs);

    QSqlQuery queryGetOrder;

    int made = 0, way = 0, deliv = 0;

    if (queryGetOrder.exec("SELECT * FROM Korzina_vseh_velosipedov WHERE id_klienta = " + QString::number(IdKlienta))){
        while (queryGetOrder.next()){
            QDate DateMade = queryGetOrder.value("data_izgotov").toDate();
            QDate DateDelivery = queryGetOrder.value("data_dostavki").toDate();
            int dayMade = Now.daysTo(DateMade);
            int dayDelivery = Now.daysTo(DateDelivery);

            QString title = queryGetOrder.value("kod_vsego_zakaza").toString();
            QListWidgetItem * newItem = new QListWidgetItem ;
            newItem -> setText(title);

            if (dayMade > 0 && dayDelivery > 0){
                ui->listWidget_Made-> insertItem(made , newItem);
                made++;
            }else if (dayMade <= 0 && dayDelivery > 0){
                ui->listWidget_OnWay -> insertItem(way , newItem);
                way++;
            } else{
                ui->listWidget_Delivery-> insertItem(deliv , newItem);
                deliv++;
            }
        }

    }

}

void MainWindowKlient::on_pushButton_clicked()  //показать
{
    SelectedBike = ui->listWidget->currentIndex().data().toString();
    QSqlQuery query;

    if (query.exec("SELECT * FROM Sklad_Velosipedov WHERE model = \'" +
                   SelectedBike + "\'")){
        if (query.next()){
            QPixmap img;
            img.loadFromData(query.value("pic").toByteArray(),"jpg");
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(img.scaled(150,130));
            ui->graphicsView->setScene(scene);
            ui->textEdit->setText(query.value("opisanie").toString());
            ui->pushButton_2->setEnabled(true);
            IdBike = query.value("id_velosipeda").toInt();
            qDebug()<<"IdBike = "<<IdBike;
            CodeBike = query.value("kod_velosipeda").toString();
            PriceSimpleBike = query.value("cena").toDouble();
        }
    }
}

void MainWindowKlient::on_pushButton_2_clicked() //добавить
{

    QSqlQuery query;

    if (query.exec("SELECT kolichestvo_na_sklade FROM Sklad_Velosipedov WHERE id_velosipeda = " +QString::number(IdBike))){
        if (query.next()){
            if (query.value(0).toInt() < ui->spinBox->value()){
                QMessageBox msgBox;
                msgBox.setText("Такое количество данной модели отсутствует на складе.\nВведите количество по-меньше.");
                msgBox.exec();
                return;
            }
        }
    }

    QSqlQuery queryOrderBike;

    if (queryOrderBike.exec("INSERT INTO ZakazVelosipeda (id_velosipeda, kod_velosipeda, kolichestvo_v_zakaze, id_klienta, cena)"
                            "VALUES (" + QString::number(IdBike) + ", \'" + CodeBike + "\', " + QString::number(ui->spinBox->value()) +"," +
                            QString::number(IdKlienta) + ", "+ QString::number(PriceSimpleBike) + ")")){
        QMessageBox msgBox;
        msgBox.setText("Велосипед добавлен в корзину.");
        msgBox.exec();


        ui->graphicsView->setScene(0);
        ui->textEdit->clear();
        ui->pushButton_2->setEnabled(false);

        UpdateBasket();

    }

}

void MainWindowKlient::on_action_triggered()
{
    this->close();
    emit firstWindow();
}

void MainWindowKlient::on_pushButton_3_clicked() //заказать
{
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

    QString CodeOrder = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString DateOrder = QDate::currentDate().toString(Qt::ISODateWithMs);
    QString DateBuilding = QDate::currentDate().addDays(20).toString(Qt::ISODateWithMs);
    QString DateDelivery = QDate::currentDate().addDays(30).toString(Qt::ISODateWithMs);
    QString IdBuildingBike = "";

    QString CodeBike = "";


    QSqlQuery queryBuildBike, queryCodeBike, queryOrderBuildingBike, queryGetCompl, queryWarehouse;
    if (queryBuildBike.exec("INSERT INTO Velosiped_sborka (id_klienta, cena_vsego) VALUES (" + QString::number(IdKlienta) + ", " + QString::number(sum) + ")")){
        if (queryCodeBike.exec("SELECT * FROM Velosiped_sborka WHERE id_velosipeda=(SELECT max(id_velosipeda) FROM Velosiped_sborka)")){
            queryCodeBike.first();
            CodeBike = queryCodeBike.value("kod_velosipeda").toString();
            IdBuildingBike = queryCodeBike.value("id_velosipeda").toString();

            for (int i = 0; i < View.size(); i++){
                if (queryGetCompl.exec("SELECT * FROM PostavlyaemoeComplect WHERE naimenovanie = \'" + View[i] + "\'")){
                    queryGetCompl.first();
                    if (queryWarehouse.exec("SELECT * FROM Sklad_Complect WHERE naimenovanie = \'" + View[i] + "\'")){
                        queryWarehouse.first();
                        QString IdCompl = queryGetCompl.value("id_complect").toString();
                        QString KodCompl = queryGetCompl.value("kod_complect").toString();
                        QString PriceCompl = queryGetCompl.value("cena").toString();
                        if (queryOrderBuildingBike.exec("INSERT INTO Complect_velosiped_po_zakazu (id_complect_na_sklade, id_complect, id_velosipeda, "
                                                        "kod_velosipeda, kod_complect, cena) "
                                                        "VALUES (" + queryWarehouse.value("id_complect_na_sklade").toString() + ", " +
                                                        IdCompl + ", " + IdBuildingBike + ", \'" +
                                                        CodeBike + "\', \'" + KodCompl + "\', " + PriceCompl + ")")){



                        }
                    }
                }
            }
            QMessageBox msgBox;
            msgBox.setText("Велосипед добавлен в корзину.");
            msgBox.exec();
        }
    }

    UpdateBasket();

}

void MainWindowKlient::UpdateBasket(){

    Products.clear();

    TotalPrice = 0;

    while (ui->tableWidget_SimpleBike->rowCount() > 0){
        ui->tableWidget_SimpleBike->removeRow(0);
    }

    while (ui->tableWidget_BuildingBike->rowCount() > 0){
        ui->tableWidget_BuildingBike->removeRow(0);
    }

    QSqlQuery querySimpleBike;
    if (querySimpleBike.exec("SELECT * FROM ZakazVelosipeda WHERE kod_zakaza = '0' AND id_klienta = " + QString::number(IdKlienta))){

        ui->tableWidget_SimpleBike->setColumnCount(3);
        QStringList name_table;
        name_table << "Модель" << "Количество" << "Цена";
        ui->tableWidget_SimpleBike->setHorizontalHeaderLabels(name_table);

        int i = 0;

        while (querySimpleBike.next()){
            ui->tableWidget_SimpleBike->insertRow( ui->tableWidget_SimpleBike->rowCount() );

            QTableWidgetItem *itm = new QTableWidgetItem(querySimpleBike.value("kod_velosipeda").toString());
            ui->tableWidget_SimpleBike->setItem(i,0,itm);
            QTableWidgetItem *itm1 = new QTableWidgetItem(querySimpleBike.value("kolichestvo_v_zakaze").toString());
            ui->tableWidget_SimpleBike->setItem(i,1,itm1);
            TotalPrice += querySimpleBike.value("cena").toDouble();
            QTableWidgetItem *itm2 = new QTableWidgetItem(querySimpleBike.value("cena").toString());
            ui->tableWidget_SimpleBike->setItem(i,2,itm2);

            Product temp;
            temp.TitleModel = querySimpleBike.value("kod_velosipeda").toString();
            temp.count = querySimpleBike.value("kolichestvo_v_zakaze").toString();
            temp.price = querySimpleBike.value("cena").toString();
            Products.push_back(temp);

            ui->tableWidget_SimpleBike->update();
            i++;

        }
    }

    QSqlQuery queryBuildingBike;
    if (queryBuildingBike.exec("SELECT * FROM Velosiped_sborka WHERE kod_vsego_zakaza = '0' AND id_klienta = " + QString::number(IdKlienta))){

        ui->tableWidget_BuildingBike->setColumnCount(3);
        QStringList name_table;
        name_table << "Модель" << "Количество" << "Цена";
        ui->tableWidget_BuildingBike->setHorizontalHeaderLabels(name_table);

        int i = 0;

        while (queryBuildingBike.next()){
            ui->tableWidget_BuildingBike->insertRow( ui->tableWidget_BuildingBike->rowCount() );

            QTableWidgetItem *itm = new QTableWidgetItem(queryBuildingBike.value("kod_velosipeda").toString());
            ui->tableWidget_BuildingBike->setItem(i,0,itm);
            QTableWidgetItem *itm1 = new QTableWidgetItem(queryBuildingBike.value("kolichestvo").toString());
            ui->tableWidget_BuildingBike->setItem(i,1,itm1);
            TotalPrice += queryBuildingBike.value("cena_vsego").toDouble();
            QTableWidgetItem *itm2 = new QTableWidgetItem(queryBuildingBike.value("cena_vsego").toString());
            ui->tableWidget_BuildingBike->setItem(i,2,itm2);

            Product temp;
            temp.TitleModel = queryBuildingBike.value("kod_velosipeda").toString();
            temp.count = queryBuildingBike.value("kolichestvo").toString();
            temp.price = queryBuildingBike.value("cena_vsego").toString();
            Products.push_back(temp);

            ui->tableWidget_BuildingBike->update();
            i++;

        }
    }

    ui->lineEdit_Total->setText(QString::number(TotalPrice));
}

void MainWindowKlient::on_pushButton_Price_clicked()
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

    QSqlQuery queryPrice;

    QString IdCompl = "";

    for (int i = 0; i < View.size(); i++){
        if (queryPrice.exec("SELECT * FROM Sklad_Complect WHERE naimenovanie = \'" + View[i] + "\'")){
            queryPrice.first();
            IdCompl = queryPrice.value("id_complect").toString();
            QSqlQuery queryPriceInPost;
            if (queryPriceInPost.exec("SELECT * FROM PostavlyaemoeComplect WHERE id_complect = " + IdCompl)){
                queryPriceInPost.first();
                sum += queryPriceInPost.value("cena").toInt();
            }
        }
    }
    int PriceWork = 10000;
    sum += PriceWork;
    ui->lineEdit_Price->setText(QString::number(sum));

    ui->pushButton_3->setEnabled(true);
}

void MainWindowKlient::on_pushButton_PayBasket_clicked()  //оплатить заказ
{
    QSqlQuery queryTotalOrder, querySimpleBike, queryBiuldingBike, querySetId;

    QString DateOrder = QDate::currentDate().toString(Qt::ISODateWithMs);
    QString DateBuilding = QDate::currentDate().addDays(20).toString(Qt::ISODateWithMs);
    QString DateDelivery = QDate::currentDate().addDays(30).toString(Qt::ISODateWithMs);

    if (queryTotalOrder.exec("INSERT INTO Korzina_vseh_velosipedov (id_klienta, data_zakaza, data_izgotov, data_dostavki, oplacheno, summa)"
                             "VALUES ("+ QString::number(IdKlienta) + ", \'" + DateOrder+"\', \'" + DateBuilding+"\', \'"+DateDelivery+
                             "\', 1, " + QString::number(TotalPrice)+") ")){


          if(querySetId.exec("SELECT * FROM Korzina_vseh_velosipedov WHERE id_vsego_zakaza=(SELECT max(id_vsego_zakaza) FROM Korzina_vseh_velosipedov)")){
              if (querySetId.first()){

              }

              if (querySimpleBike.exec("UPDATE ZakazVelosipeda SET id_vsego_zakaza = " + querySetId.value("id_vsego_zakaza").toString()+
                                       ", kod_zakaza = \'" + querySetId.value("kod_vsego_zakaza").toString() +
                                       "\' WHERE id_klienta = " + QString::number(IdKlienta) + " AND kod_zakaza = \'0\'")){

              }

              if (queryBiuldingBike.exec("UPDATE Velosiped_sborka SET id_vsego_zakaza = " + querySetId.value("id_vsego_zakaza").toString()+
                                         ", kod_vsego_zakaza = \'" + querySetId.value("kod_vsego_zakaza").toString() +
                                         "\' WHERE id_klienta = " + QString::number(IdKlienta) + " AND kod_vsego_zakaza = \'0\'")){

              }
          }
    }

    QSqlQuery queryPayment;
    double TaxSumma = 1.2 * TotalPrice;
    if (queryPayment.exec("INSERT INTO Schet_predpriatia (summa_s_nds, id_vsego_zakaza) "
                          "VALUES (" + QString::number(TaxSumma) + ", " + querySetId.value("id_vsego_zakaza").toString() + ")")){


    }


        QSqlQuery queryCompany;

        if (queryCompany.exec("SELECT * FROM Predpriatie WHERE id_predpriatia = " + IdPred)){
            queryCompany.first();
        }

        QSqlQuery queryClient;

        if (queryClient.exec("SELECT * FROM Klient WHERE id_klienta = " + QString::number(IdKlienta))){
            queryClient.first();
        }

        QString html =
        "<h1 align=center>"
        "Документ об оплате товаров <br>№ " + querySetId.value("kod_vsego_zakaza").toString()+"</h1>"
        "<p align=justify>"
        "Предприятие: " + queryCompany.value("kod_predpriatia").toString()+"<br>"
        "Директор: " + queryCompany.value("fio_directora").toString()+"<br>"
        "Заказчик: " + queryClient.value("fio_klienta").toString()+"<br>"
        "Составляющие заказа: <br>";

        for (int i = 0; i < Products.size(); i++){
            html += QString::number(1+i);
            html += ") Велосипед - ";
            html += Products[i].TitleModel;
            html += "; цена за штуку - ";
            html += Products[i].price;
            html += "рублей ; количество - ";
            html += Products[i].count;
            html += "<br>";
        }

        html += "Дата оформления заказа: " + DateOrder+"<br>"
        "Дата изготовления: " + DateBuilding+"<br>"
        "Дата доставки: " + DateDelivery+"<br>"
        "Сумма заказа: " + QString::number(sum) + "<br>"
        "Итог (с налогом): " + QString::number(TaxSumma) + "<br>"
        "Заказ оплачен.<br>"
        "</p>"
        "<div align=right>IS</div>";

        QTextDocument document;
        document.setHtml(html);

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        QString temp =  querySetId.value("kod_vsego_zakaza").toString();
        QString FileName = "C:/Users/User/Desktop/bd/Payment_doc_client/"+temp+".pdf";
        qDebug()<<FileName;
        printer.setOutputFileName(FileName);
        printer.setPageMargins(QMarginsF(15, 15, 15, 15));

        document.print(&printer);

        QMessageBox msgBox;
        msgBox.setText("Заказ оплачен.\nДокумент сохранен в "+temp+".pdf");
        msgBox.exec();

        UpdateBasket();
        UpdateStatusOrder();

}


