#ifndef ZAKAZVELOSIPEDA_H
#define ZAKAZVELOSIPEDA_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QDate>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class ZakazVelosipeda;
}

class ZakazVelosipeda : public QMainWindow
{
    Q_OBJECT

public:
    explicit ZakazVelosipeda(int idKlienta, int idBike, QWidget *parent = nullptr);
    ~ZakazVelosipeda();

signals:
    void firstWindow();

private:
    Ui::ZakazVelosipeda *ui;

    int IdKlienta = 0;
    int IdBike = 0;


};

#endif // ZAKAZVELOSIPEDA_H
