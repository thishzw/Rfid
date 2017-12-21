#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QSqlError>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_2->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();
    ui->lineEdit_5->hide();
    ui->pushButton_3->hide();
    add=false;
    isCustomer=false;
    identify=false;
}
void MainWindow::setCom(Reader *com)
{
    comReader=com;
    comReader->openCOMM(4);
}
void MainWindow::setDataBase(QSqlDatabase *database)
{
    db=database;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mode()
{

    ui->lineEdit_2->setReadOnly(false);
    ui->lineEdit_3->setReadOnly(false);
    ui->lineEdit_4->setReadOnly(false);
    ui->lineEdit_5->setReadOnly(false);
    ui->label->show();
    ui->lineEdit->show();
    ui->label_2->show();
    ui->lineEdit_2->show();
    ui->label_3->show();
    ui->lineEdit_4->show();
    ui->pushButton_2->show();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    if(isCustomer)
    {
        ui->label_4->show();
        ui->label_5->show();
        ui->lineEdit_3->show();
        ui->lineEdit_5->show();
        ui->label_2->setText("姓名：");
        ui->label_3->setText("性别：");
        ui->label_2->repaint();
        ui->label_3->repaint();

    }
    else
    {
        ui->label_4->hide();
        ui->label_5->hide();
        ui->lineEdit_3->hide();
        ui->lineEdit_5->hide();
        ui->label_2->setText("商品名：");
        ui->label_2->repaint();
        ui->label_3->setText("售价：");
        ui->label_3->repaint();

    }
}
void MainWindow::WriteCustomer()
{

}

void MainWindow::WriteGoods()
{

}


void MainWindow::on_pushButton_clicked()
{
    //识别卡获得EPC
    if(!comReader->IdentifySingleTag(EPC,nullptr,0))
    {
        QMessageBox::warning(this,"ERROR","IDENTFY EPC ERROR",QMessageBox::Ok);
        return;
    }
    QByteArray array;
    array.setRawData(EPC,12);
    array=array.toHex();
    QString epc(array);
    qDebug()<<epc;
    QString temp="SELECT * FROM card WHERE EPC=\'";
    temp+=epc;
    temp+="\'";
    QSqlQuery query(*db);
    query.clear();
    if(!query.exec(temp))
        QMessageBox::warning(this,"ERROR","SQL SELECT ERROR",QMessageBox::Ok);
       query.next();
    if(query.size())
    {
       //已被标识的卡
        if(add)
        {
            QMessageBox::warning(this,"ERROR","THIS LABEL IS NOT A NEW ONE");
            return;
        }
        ui->pushButton_3->show();
        ui->label_7->setText("已检测到标签");
        ui->label_7->repaint();
        QString type=query.value("type").toString();
        if(type=="customer")
            isCustomer=true;
        else
            isCustomer=false;
        cardID=query.value("id").toInt();
        mode();
        ui->lineEdit->setText(QString::number(cardID,10));
        ui->lineEdit->repaint();
        temp="SELECT * FROM ";
        if(isCustomer)
            temp+="customer WHERE id=";
        else
            temp+="goods WHERE id=";
        temp+=QString::number(cardID,10);
        query.clear();
        if(!query.exec(temp))
            QMessageBox::warning(this,"ERROR","SQL SELECT customer ERROR",QMessageBox::Ok);
        query.next();
        if(isCustomer)
        {
            ui->lineEdit_2->setText(query.value("name").toString());
            ui->lineEdit_4->setText(query.value("male").toString());
            ui->lineEdit_3->setText(QString::number(query.value("age").toInt()));
            ui->lineEdit_5->setText(QString::number(query.value("money").toDouble(),10,2));
            ui->lineEdit_2->repaint();
            ui->lineEdit_3->repaint();
            ui->lineEdit_4->repaint();
            ui->lineEdit_5->repaint();
        }
        else
        {
             ui->lineEdit_2->setText(query.value("name").toString());
             ui->lineEdit_2->repaint();
             ui->lineEdit_4->setText(QString::number(query.value("price").toDouble(),10,2));
             ui->lineEdit_4->repaint();
        }
        identify=true;
        ui->pushButton_2->show();
    }
    else
    {
        identify=true;
        if(!add)
        QMessageBox::about(this,"SUCCESS","THIS IS A NEW LABEL,PLEASE CHOOSE ADD A GOODS OR CUSTOMER");
    }
    comReader->CloseCOMM();
    comReader->openCOMM(4);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!identify)
        QMessageBox::warning(this,"ERROR","identify FRIST",QMessageBox::Ok);
    else
    {
        QSqlQuery query(*db);
        QByteArray array;
        array.setRawData(EPC,12);
        array=array.toHex();
        QString epc(array);
        if(add)
        {
           //新增的卡片
            QString temp="INSERT INTO card (EPC,type) values(\'";
            temp+=epc;
            temp+="\',\'";
            if(isCustomer)
                temp+="customer\')";
            else
                temp+="goods\')";
            if(!query.exec(temp))
                QMessageBox::warning(this,"ERROR","SQL INSERT  ERROR",QMessageBox::Ok);
            temp="SELECT id FROM card WHERE EPC=\'";
            temp+=epc;
            temp+="\'";
            query.clear();
            if(!query.exec(temp))
            QMessageBox::warning(this,"ERROR","SQL  SAVE SELECT  ERROR",QMessageBox::Ok);
            query.next();
            cardID=query.value("id").toInt();
            ui->lineEdit->setText(QString::number(cardID,10));
            ui->lineEdit->repaint();
            query.clear();
            if(isCustomer)
            {
                ui->lineEdit_5->setText("0.00");
                ui->label_5->repaint();
                QString name =ui->lineEdit_2->text();
                QString male=ui->lineEdit_4->text();
                int age=ui->lineEdit_3->text().toInt(nullptr,10);
                ui->lineEdit_5->setReadOnly(true);
                temp="INSERT INTO customer (id,name,male,age,money) values(";
                temp+=QString::number(cardID,10);
                temp+=",\'";
                temp+=name;
                temp+="\',\'";
                temp+=male;
                temp+="\',";
                temp+=QString::number(age,10);
                temp+=",0.00)";
                if(!query.exec(temp))
                {
                    QMessageBox::warning(this,"ERROR","SQL  SAVE INSERT  ERROR",QMessageBox::Ok);
                    return;
                }
                QMessageBox::about(this,"SUCCESS","ADD A NEW LABEL");
            }
            else
            {
                QString name =ui->lineEdit_2->text();
                double price=ui->lineEdit_4->text().toDouble();
                temp="INSERT INTO goods (id,name,price) values(";
                temp+=QString::number(cardID,10);
                temp+=",\'";
                temp+=name;
                temp+="\',";
                temp+=QString::number(price,10,2);
                temp+=")";
                if(!query.exec(temp))
                {
                    QMessageBox::warning(this,"ERROR","SQL  SAVE INSERT  ERROR",QMessageBox::Ok);
                   qDebug()<<query.lastError().text();
                    return;
                }
                QMessageBox::about(this,"SUCCESS","ADD A NEW LABEL");
            }
        }
        else
        {
           //旧卡片
            QString temp;
            temp="SELECT id FROM card WHERE EPC=\'";
            temp+=epc;
            temp+="\'";
            query.clear();
            if(!query.exec(temp))
            QMessageBox::warning(this,"ERROR","SQL  SAVE SELECT  ERROR",QMessageBox::Ok);
            query.next();
            cardID=query.value("id").toInt();
            query.clear();
           if(isCustomer)
           {
               QString name =ui->lineEdit_2->text();
               QString male=ui->lineEdit_4->text();
               int age=ui->lineEdit_3->text().toInt(nullptr,10);
               temp="UPDATE customer SET name=\'";
               temp+=name;
               temp+="\',male=\'";
               temp+=male;
               temp+="\',age=";
               temp+=QString::number(age,10);
               temp+=",money=";
               temp+=QString::number(ui->lineEdit_5->text().toDouble(),10,2);
               temp+=" WHERE id=";
               temp+=QString::number(cardID,10);
               query.clear();
               if(!query.exec(temp))
                  QMessageBox::warning(this,"ERROR","SQL  SAVE UPDATE  ERROR",QMessageBox::Ok);
               else
                  QMessageBox::about(this,"SUCCESS","UPDATE A NEW LABEL");
           }
        }
    }
    add=false;
    identify=false;
    ui->pushButton_3->hide();
    ui->label_7->setText("未检测到标签");
    ui->label_7->repaint();
}

void MainWindow::on_action_triggered()
{
    isCustomer=true;
    add=true;
    mode();
}

void MainWindow::on_actionAdd_triggered()
{
    add=true;
    isCustomer=false;
    mode();
}

void MainWindow::on_pushButton_3_clicked()
{
    QSqlQuery query(*db);
    QString temp="DELETE FROM card WHERE id=" ;
    temp+=QString::number(cardID,10);
    if(!query.exec(temp))
    {
        QMessageBox::warning(this,"ERROR","SQL  DELETE CARD  ERROR",QMessageBox::Ok);
        return;
    }
    query.clear();
    temp="DELETE FROM ";
    if(isCustomer)
        temp+="customer WHERE id="  ;
    else
        temp+="goods WHERE id=";
    temp+=QString::number(cardID,10);
    if(!query.exec(temp))
    {
        QMessageBox::warning(this,"ERROR","SQL  DELETE CARD  ERROR",QMessageBox::Ok);
        return;
    }
    else
    {
       QMessageBox::about(this,"SUCCESS","DELETE A LABEL");
       return;
    }
}
