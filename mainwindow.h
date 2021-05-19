#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

#include "windowregklient.h"
#include "windowklient.h"
#include "windowpostavsika.h"
#include "windowpredpriatia.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_Pred_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase bd;

    QString KlientLogin = "";
    QString KlientParol = "";
    QString KlientFIO = "";

    int IdKlienta = 0;
    int IdPostavsika = 0;
    int IdPredpriatia = 0;

    MainWindowRegKlient *WindowRegKlient;
    MainWindowKlient *WindowKlient;
    WindowPostavsika *WinPostavsika;
    WindowPredpriatia *WinPred;
};
#endif // MAINWINDOW_H
