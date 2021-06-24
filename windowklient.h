#ifndef MAINWINDOWKLIENT_H
#define MAINWINDOWKLIENT_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QListWidgetItem>
#include <QDateTime>
#include <QDate>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

#include "zakazvelosipeda.h"

struct Product{
    QString price = "";
    QString count = "";
    QString TitleModel = "";
};

struct ComplectBike{
    QString privod = "";
    QString uprav = "";
    QString pedal = "";
    QString tormoz = "";
    QString kolesa = "";
    QString rezina = "";
    QString tros = "";
    QString ramki = "";
};

namespace Ui {
class MainWindowKlient;
}

class MainWindowKlient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowKlient(int idKlienta, QWidget *parent = nullptr);
    ~MainWindowKlient();

signals:
    void firstWindow();

private slots:

    void on_action_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_Price_clicked();

    void UpdateBasket();
    void UpdateStatusOrder();

    void on_pushButton_PayBasket_clicked();

    void on_listWidget_Schema_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_5_clicked();

private:
    Ui::MainWindowKlient *ui;

    ZakazVelosipeda *Bike;

    QString SelectedBike;
    QString IdPred = "1";
    int IdKlienta = 0;
    int IdBike = 0;
    double PriceSimpleBike = 0;
    QString CodeBike = "";
    int sum = 0;
    double TotalPrice = 0;

    QVector <Product> Products;
    QVector <QString> ComplBike;
};

#endif // MAINWINDOWKLIENT_H
