#ifndef MAINWINDOWKLIENT_H
#define MAINWINDOWKLIENT_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class MainWindowKlient;
}

class MainWindowKlient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowKlient(QWidget *parent = nullptr);
    ~MainWindowKlient();

signals:
    void firstWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindowKlient *ui;

    QString SelectedBike;
};

#endif // MAINWINDOWKLIENT_H
