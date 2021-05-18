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


    queryZakaz.exec("INSERT INTO ZakazVelosipeda (id_klienta, id_velosipeda, kod_zakaza, kod_velosipeda,"
                    "data_zakaza, data_izgotov, data_dostavki, cena) "
                    "VALUES (" + QString::number(IdKlienta) + "," + QString::number(IdBike) + "," + "\'З - \'"
                    +", \'" + queryVel.value("kod_velosipeda").toString() + "\', \'" +QDate::currentDate().toString(Qt::ISODate)+"\',"
                    + " \'" +QDate::currentDate().addDays(3).toString(Qt::ISODate)+"\',"+ " \'" +QDate::currentDate().addDays(6).toString(Qt::ISODate)+"\',"
                    + "\'" + queryVel.value("cena").toString() + "\'" +")");


//    if (query.exec("SELECT * FROM Velosiped WHERE id_velosipeda = " + QString::number(IdBike) )){
//        if (query.next()){
//            QPixmap img;
//            img.loadFromData(query.value("pic").toByteArray(),"jpg");
//            QGraphicsScene *scene = new QGraphicsScene;
//            scene->addPixmap(img.scaled(150,130));
//            ui->graphicsView->setScene(scene);

//            ui->textEdit->setText(query.value("opisanie").toString());
//            IdBike = query.value("id_velosipeda").toInt();
//        }
//    }
}

ZakazVelosipeda::~ZakazVelosipeda()
{
    delete ui;
}
