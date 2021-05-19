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

private:
    Ui::WindowPredpriatia *ui;

    int IdPred = 0;
};

#endif // WINDOWPREDPRIATIA_H
