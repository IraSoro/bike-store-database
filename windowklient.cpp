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

    if (query.exec("SELECT pic FROM Velosiped WHERE model = \'" +
                   SelectedBike + "\'")){
        if (query.next()){
            qDebug()<<"1111"<<query.value(0);
            QGraphicsScene *scene = new QGraphicsScene;
            scene->addPixmap(QPixmap(query.value(0).toString()));
            scene->addText("test");
            ui->graphicsView->setScene(scene);
        }
    }

}
