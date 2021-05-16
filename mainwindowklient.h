#ifndef MAINWINDOWKLIENT_H
#define MAINWINDOWKLIENT_H

#include <QMainWindow>

namespace Ui {
class MainWindowKlient;
}

class MainWindowKlient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowKlient(QWidget *parent = nullptr);
    ~MainWindowKlient();

private:
    Ui::MainWindowKlient *ui;
};

#endif // MAINWINDOWKLIENT_H
