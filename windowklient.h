#ifndef MAINWINDOWKLIENT_H
#define MAINWINDOWKLIENT_H

#include <QMainWindow>
#include <QListWidgetItem>

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

private:
    Ui::MainWindowKlient *ui;
};

#endif // MAINWINDOWKLIENT_H
