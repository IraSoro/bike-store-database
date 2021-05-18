#include "zakazvelosipeda.h"
#include "ui_zakazvelosipeda.h"

ZakazVelosipeda::ZakazVelosipeda(int idKlienta, int idBike, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ZakazVelosipeda)
{
    ui->setupUi(this);
    setWindowTitle("Оформление заказа");

    IdKlienta = idKlienta;
    IdBike = idBike;

    QSqlQuery queryVel, queryKlient, queryZakaz;

    qDebug()<<IdBike<<"-"<<IdKlienta;

    queryVel.exec("SELECT * FROM Velosiped WHERE id_velosipeda = " + QString::number(IdBike));
    queryVel.first();
    queryKlient.exec("SELECT * FROM Klient WHERE id_klienta = " + QString::number(IdKlienta));
    queryKlient.first();

    QString KodZakaza = "ЗВ - "+QDateTime::currentDateTime().toString(Qt::ISODate);

    queryZakaz.exec("INSERT INTO ZakazVelosipeda (id_klienta, id_velosipeda, kod_zakaza, kod_velosipeda,"
                    "data_zakaza, data_izgotov, data_dostavki, cena) "
                    "VALUES (" + QString::number(IdKlienta) + "," + QString::number(IdBike) + "," + "\'" + KodZakaza+ "\'"
                    +", \'" + queryVel.value("kod_velosipeda").toString() + "\', \'" +QDate::currentDate().toString(Qt::ISODate)+"\',"
                    + " \'" +QDate::currentDate().addDays(3).toString(Qt::ISODate)+"\',"+ " \'" +QDate::currentDate().addDays(6).toString(Qt::ISODate)+"\',"
                    + "\'" + queryVel.value("cena").toString() + "\'" +")");


    if (queryZakaz.exec("SELECT * FROM ZakazVelosipeda WHERE kod_zakaza = \'" + KodZakaza + "\'" )){
        if (queryZakaz.first()){
            QPixmap img;
            img.loadFromData(queryVel.value("pic").toByteArray(),"jpg");
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(img.scaled(150,130));
            ui->graphicsView->setScene(scene);

            ui->textEdit->insertPlainText("Заказ № " + KodZakaza + "\n");
            ui->textEdit->insertPlainText("Заказчик: " + queryKlient.value("fio_klienta").toString() + "\n");
            ui->textEdit->insertPlainText("Модель велосипеда: " + queryVel.value("model").toString() + "\n");
            ui->textEdit->insertPlainText("Код велосипеда: " + queryVel.value("kod_velosipeda").toString() + "\n");
            ui->textEdit->insertPlainText("Дата оформления заказа: " + queryZakaz.value("data_zakaza").toString() + "\n");
            ui->textEdit->insertPlainText("Дата изготовления: " + queryZakaz.value("data_izgotov").toString() + "\n");
            ui->textEdit->insertPlainText("Дата доставки: " + queryZakaz.value("data_dostavki").toString() + "\n");
            ui->textEdit->insertPlainText("Цена: " + queryZakaz.value("cena").toString() + "\n");

        }
    }

    html =
    "<h1 align=center>"
    "Заказ № " + KodZakaza+"</h1>"
    "<p align=justify>"
    "Заказчик: " + queryKlient.value("fio_klienta").toString()+"<br>"
    "Модель велосипеда: " + queryVel.value("model").toString()+"<br>"
    "Код велосипеда: " + queryVel.value("kod_velosipeda").toString()+"<br>"
    "Дата оформления заказа: " + queryZakaz.value("data_zakaza").toString()+"<br>"
    "Дата изготовления: " + queryZakaz.value("data_izgotov").toString()+"<br>"
    "Дата доставки: " + queryZakaz.value("data_dostavki").toString()+"<br>"
    "Цена: " + queryZakaz.value("cena").toString()+"<br>"
    "</p>"
    "<div align=right>IS</div>";
}

ZakazVelosipeda::~ZakazVelosipeda()
{
    delete ui;
}

void ZakazVelosipeda::on_pushButton_clicked()
{
    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("C:/Users/User/Desktop/bd/zakaz.pdf");
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    this->close();
    emit PersonWindow();
}
