#ifndef WINDOWPOSTAVSIKA_H
#define WINDOWPOSTAVSIKA_H

#include <QMainWindow>

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

private:
    Ui::WindowPostavsika *ui;
};

#endif // WINDOWPOSTAVSIKA_H
