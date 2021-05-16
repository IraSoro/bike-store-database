#ifndef MAINWINDOWREGKLIENT_H
#define MAINWINDOWREGKLIENT_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class MainWindowRegKlient;
}

class MainWindowRegKlient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowRegKlient(QWidget *parent = nullptr);
    ~MainWindowRegKlient();

signals:
    void firstWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindowRegKlient *ui;
};

#endif // MAINWINDOWREGKLIENT_H
