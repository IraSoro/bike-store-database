#ifndef WINDOWPREDPRIATIA_H
#define WINDOWPREDPRIATIA_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QTabWidget>
#include <QFileDialog>
#include <QDateTime>
#include <QDate>
#include <QListWidgetItem>
#include <QPrinter>
#include <QTextDocument>

#include "QtSql/QSqlDatabase"
#include "QSqlQuery"

namespace Ui {
class WindowPredpriatia;
}

class WindowPredpriatia : public QMainWindow
{
    Q_OBJECT

signals:
    void firstWindow();

public:
    explicit WindowPredpriatia(int idPred, QWidget *parent = nullptr);
    ~WindowPredpriatia();

private slots:
    void on_pushButton_add_post_clicked();

    void on_pushButton_ShowKat_clicked();

    void on_pushButton_ShowModel_clicked();

    void on_pushButton_AddCount_clicked();

    void on_pushButton_ShowBasket_clicked();

    void on_pushButton_Pay_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_action_triggered();

private:
    Ui::WindowPredpriatia *ui;

    int IdPred = 0;
    int IdSelectedPost = 0;
    QString IdSelectedPostInBasket = "";
    QString TitleModel = "<br>";
    int sum = 0;


    void ConclusionNewContract(QDate start, QDate finish, QString KodDogovora);
    void UpdateForm();

};

#endif // WINDOWPREDPRIATIA_H
