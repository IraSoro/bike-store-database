#include "windowklient.h"
#include "ui_windowklient.h"

MainWindowKlient::MainWindowKlient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowKlient)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет");

    QListWidgetItem * newItem = new QListWidgetItem ;
    newItem -> setText("itemText");
    ui->listWidget -> insertItem(1 , newItem);


}

MainWindowKlient::~MainWindowKlient()
{
    delete ui;
}
