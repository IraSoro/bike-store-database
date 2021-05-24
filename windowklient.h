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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_Price_clicked();

private:
    Ui::MainWindowKlient *ui;

    ZakazVelosipeda *Bike;

    QString SelectedBike;
    QString IdPred = "1";
    int IdKlienta = 0;
    int IdBike = 0;
    int sum = 0;
};

#endif // MAINWINDOWKLIENT_H
