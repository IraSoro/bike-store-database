#include "windowklient.h"
#include "ui_windowklient.h"

// TODO: заказ комплектующих

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

void MainWindowKlient::on_action_triggered()
{
    this->close();
    emit firstWindow();
}

void MainWindowKlient::on_pushButton_3_clicked()
{



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
