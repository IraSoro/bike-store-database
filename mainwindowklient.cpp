#include "mainwindowklient.h"
#include "ui_mainwindowklient.h"

MainWindowKlient::MainWindowKlient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowKlient)
{
    ui->setupUi(this);
}

MainWindowKlient::~MainWindowKlient()
{
    delete ui;
}
