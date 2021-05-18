#include "windowklient.h"
#include "ui_windowklient.h"

MainWindowKlient::MainWindowKlient( int idKlienta, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowKlient)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет");

    IdKlienta = idKlienta;

    QSqlQuery query;

    if (query.exec("SELECT model FROM Velosiped")){
        int i = 0;
        while (query.next()){
            QListWidgetItem * newItem = new QListWidgetItem ;
            newItem -> setText(query.value(0).toString());
            ui->listWidget -> insertItem(i , newItem);
            i++;
        }
    }

    ui->pushButton_2->setEnabled(false);
}

MainWindowKlient::~MainWindowKlient()
{
    delete ui;
}

void MainWindowKlient::on_pushButton_clicked()  //показать
{
    SelectedBike = ui->listWidget->currentIndex().data().toString();
    QSqlQuery query;

    if (query.exec("SELECT pic, opisanie, id_velosipeda FROM Velosiped WHERE model = \'" +
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
        }
    }
}

void MainWindowKlient::on_pushButton_2_clicked() //заказать
{
    QSqlQuery query;

    if (query.exec("SELECT kolichestvo FROM Sklad_Velosipedov WHERE id_velosipeda = " +QString::number(IdBike))){
        if (query.next()){
            if (query.value(0).toInt() == 0){
                QMessageBox msgBox;
                msgBox.setText("Данная модель отсутствует на складе.");
                msgBox.exec();
                return;
            }
        }
    }

    Bike = new ZakazVelosipeda(IdKlienta, IdBike);
    connect(Bike, &ZakazVelosipeda::PersonWindow, this, &ZakazVelosipeda::show);
    Bike->show();
    this->close();
}
