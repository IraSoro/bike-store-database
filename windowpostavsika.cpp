#include "windowpostavsika.h"
#include "ui_windowpostavsika.h"

//TODO: проверка поступления новых заказов, выполнение заказов

WindowPostavsika::WindowPostavsika(int idPostavsika, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowPostavsika)
{
    ui->setupUi(this);
    setWindowTitle("Личный кабинет поставщика");
    if (ui->tabWidget->currentIndex() == 1){

    }

    IdPostavsika = idPostavsika;


    QSqlQuery queryOrder;

    int i = 0;
    if (queryOrder.exec("SELECT * FROM Zakaz_complect WHERE oplata = 1 AND prinat = 0 AND id_postavsika = " + QString::number(IdPostavsika))){
        while (queryOrder.next()){
            QString name = queryOrder.value("kod_zakaza").toString();
            QListWidgetItem * newItem = new QListWidgetItem ;

            bool found = false;
            for (int j = 0; j < ui->listWidget_ShowOrder->count(); ++j) {
                if (ui->listWidget_ShowOrder->item(j)->data(Qt::DisplayRole).toString() == name) {
                    found = true;
                    break;
                }
            }

            if (!found){
                newItem -> setText(name);
                ui->listWidget_ShowOrder-> insertItem(i , newItem);
                i++;
            }

            i++;
        }
    }

    ui->pushButton_Execute->setEnabled(false);


}

WindowPostavsika::~WindowPostavsika()
{
    delete ui;
}

void WindowPostavsika::on_pushButton_Add_clicked()
{
    QString LineEditName = ui->lineEdit_Name->text();
    QString LineEditPrice = ui->lineEdit_Price->text();
    QString LineEditPic = ui->lineEdit_Pic->text();
    QString ComboBoxCategory = ui->comboBoxC->currentText();

    if (LineEditName == "" || LineEditPrice == "" /*|| LineEditPic == ""*/){
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
                    "naimenovanie, kategoria, cena) "
                    "VALUES (" + QString::number(IdPostavsika) + ", \'"+ KodCompl + "\',\'" +
                    queryPos.value("kod_postavsika").toString()+"\', \'" +LineEditName + "\', \'" +
                    ComboBoxCategory + "\'," + LineEditPrice  +")" );

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

void WindowPostavsika::on_pushButton_Show_clicked() //показать заказы
{
    ui->pushButton_Execute->setEnabled(true);

    while (ui->tableWidget_OrderPost->rowCount() > 0){
        ui->tableWidget_OrderPost->removeRow(0);
    }

    QString kodOrder = ui->listWidget_ShowOrder->currentIndex().data().toString();
    if (kodOrder == ""){
        QMessageBox msgBox;
        msgBox.setText("Заказ не выбран.");
        msgBox.exec();
        return;
    }

    QSqlQuery queryOrder;

    if (queryOrder.exec("SELECT * FROM Zakaz_complect WHERE oplata = 1 AND prinat = 0 AND kod_zakaza = \'" +kodOrder+"\'")){
        ui->tableWidget_OrderPost->setColumnCount(3);
        QStringList name_table;
        name_table << "Категория" << "Комплектующее" << "Количество";
        ui->tableWidget_OrderPost->setHorizontalHeaderLabels(name_table);

        int i = 0;
        while (queryOrder.next()){
            ui->tableWidget_OrderPost->insertRow( ui->tableWidget_OrderPost->rowCount() );

            QSqlQuery queryCompl;
            if (queryCompl.exec("SELECT * FROM PostavlyaemoeComplect WHERE kod_complect = \'" + queryOrder.value("kod_complect").toString()+"\'")){
                queryCompl.first();
                QTableWidgetItem *itm = new QTableWidgetItem(queryCompl.value("kategoria").toString());
                ui->tableWidget_OrderPost->setItem(i,0,itm);
                QTableWidgetItem *itm1 = new QTableWidgetItem(queryCompl.value("naimenovanie").toString());
                ui->tableWidget_OrderPost->setItem(i,1,itm1);
                QTableWidgetItem *itm2 = new QTableWidgetItem(queryOrder.value("kolichestvo").toString());
                ui->tableWidget_OrderPost->setItem(i,2,itm2);

                ui->tableWidget_OrderPost->update();
                i++;
            }
        }
    }


}
