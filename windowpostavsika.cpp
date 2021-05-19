#include "windowpostavsika.h"
#include "ui_windowpostavsika.h"

WindowPostavsika::WindowPostavsika(int idPostavsika, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPostavsika)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет");
    if (ui->tabWidget->currentIndex() == 1){

    }

    IdPostavsika = idPostavsika;
}

WindowPostavsika::~WindowPostavsika()
{
    delete ui;
}

void WindowPostavsika::on_pushButton_Add_clicked()
{
    QString LineEditName = ui->lineEdit_Name->text();
    int SpinBoxCount = ui->spinBox_Count->value();
    QString LineEditPrice = ui->lineEdit_Price->text();
    QString LineEditPic = ui->lineEdit_Pic->text();
    QString ComboBoxCategory = ui->comboBoxC->currentText();

    if (LineEditName == "" || LineEditPrice == "" || LineEditPic == "" || SpinBoxCount == 0){
        QMessageBox msgBox;
        msgBox.setText("Вы ввели не все данные.");
        msgBox.exec();
        return;
    }


    QSqlQuery queryPos, queryCompl;

    queryPos.exec("SELECT * FROM Postavsik WHERE id_postavsika = " + QString::number(IdPostavsika));
    queryPos.first();

    QString KodCompl = "К - "+QDateTime::currentDateTime().toString(Qt::ISODate);

    queryCompl.exec("INSERT INTO PostavlyaemoeComplect (id_postavsika, kod_complect, kod_postavsika,"
                    "naimenovanie, kategoria, kolichestvo, cena) "
                    "VALUES (" + QString::number(IdPostavsika) + ", \'"+ KodCompl + "\',\'" +
                    queryPos.value("kod_postavsika").toString()+"\', \'" +LineEditName + "\', \'" +
                    ComboBoxCategory + "\'," + QString::number(SpinBoxCount)+ ", " + LineEditPrice  +")" );

    QMessageBox msgBox;
    msgBox.setText("Введенное комплектующее успешно добавлено");
    msgBox.exec();
    return;



}

void WindowPostavsika::on_pushButton_Open_clicked()
{
    FileName = QFileDialog::getOpenFileName(this,tr("Select video"),"C:/Users/User/Desktop/bd",tr("Video files (*.jpg)"));
    ui->lineEdit_Pic->setText(FileName);
}
