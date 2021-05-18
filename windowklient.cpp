#include "windowklient.h"
#include "ui_windowklient.h"

MainWindowKlient::MainWindowKlient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowKlient)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет");

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

}

MainWindowKlient::~MainWindowKlient()
{
    delete ui;
}

void MainWindowKlient::on_pushButton_clicked()
{
    SelectedBike = ui->listWidget->currentIndex().data().toString();
    QSqlQuery query;

    if (query.exec("SELECT pic, opisanie FROM Velosiped WHERE model = \'" +
                   SelectedBike + "\'")){
        if (query.next()){
            QPixmap img;
            img.loadFromData(query.value("pic").toByteArray(),"jpg");
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(img.scaled(150,130));
            ui->graphicsView->setScene(scene);
            ui->textEdit->setText(query.value("opisanie").toString());
        }
    }
}
