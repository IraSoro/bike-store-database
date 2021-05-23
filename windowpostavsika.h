#ifndef WINDOWPOSTAVSIKA_H
#define WINDOWPOSTAVSIKA_H

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
class WindowPostavsika;
}

class WindowPostavsika : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowPostavsika(int idPostavsika, QWidget *parent = nullptr);
    ~WindowPostavsika();

signals:
    void firstWindow();

private slots:
    void on_pushButton_Add_clicked();

    void on_pushButton_Open_clicked();

    void on_pushButton_Show_clicked();

    void on_pushButton_Execute_clicked();

private:
    Ui::WindowPostavsika *ui;

    QString FileName = "";
    int IdPostavsika = 0;
    int IdPred = 1;
    int row = 0;

    void UpdateForm();
};

#endif // WINDOWPOSTAVSIKA_H
