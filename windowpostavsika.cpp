#include "windowpostavsika.h"
#include "ui_windowpostavsika.h"

WindowPostavsika::WindowPostavsika(int idPostavsika, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPostavsika)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет");
}

WindowPostavsika::~WindowPostavsika()
{
    delete ui;
}
