#include "client.h"
#include "ui_client.h"
#include"pay.h"
#include<QDebug>
Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    total=0;
    ui->pushButton_2->hide();
    goodsID.clear();
    customer=false;
}
void Client::setCom(int no)
{
    this->no=no;
    com=OpenComm(no);
}
Client::~Client()
{
    delete ui;
}
void Client::setDataBase(QSqlDatabase *database)
{
    db=database;
}

void Client::on_pushButton_clicked()
{
    if(!IdentifyUploadedSingleTag(com,EPC,nullptr,0))
    {
        QMessageBox::warning(this,"ERROR","IdentifyTag ERROR");
        CloseComm(com);
        com=OpenComm(no);
        return;
    }
    QSqlQuery query(*db);
    QString temp="SELECT * FROM card WHERE EPC=\'";
        query.clear();
        QByteArray array;
        char temp2[12];
        for(int i=0;i<12;i++)
        {
            temp2[i]=(int)EPC[i];
        }
        array.setRawData(temp2,12);
        array=array.toHex();
        QString epc(array);
        qDebug()<<epc;
        QString temp1=temp+epc;
        temp1+="\'";
        if(!query.exec(temp1))
        {
            QMessageBox::warning(this,"ERROR","Identify SQL ERROR");
            CloseComm(com);
            com=OpenComm(no);
            return;
        }
        qDebug()<<"ddddddddddd"<<query.size();
        if(query.size()==0)
        {
            QMessageBox::warning(this,"ERROR","Label isempty");
            CloseComm(com);
            com=OpenComm(no);
            return;
        }
        query.next();
        if(query.value("type").toString()=="customer")
        {
            customerID=query.value("id").toInt();
            SetCustomer(customerID);
            customer=true;
        }
        else
        {
            AddAGoods(query.value("id").toInt());
        }
    ui->pushButton_2->show();
    CloseComm(com);
    com=OpenComm(no);
}

void Client::on_pushButton_2_clicked()
{
    if(customer)
    {
        QSqlQuery query(*db);
        QString temp="UPDATE customer SET money=";
        double temp1=nowMoney-total;
        qDebug()<<"nowMoney"<<nowMoney;
        if(temp1<0)
        {
            QMessageBox::warning(this,"ERROR","YOU DON'T HAVE ENOUGH MONEY");
            return;
        }
        temp+=QString::number(temp1,10,2);
        temp+=" WHERE id=";
        temp+=QString::number(customerID,10);
        if(!query.exec(temp))
        {
            QMessageBox::warning(this,"ERROR","UPDATE SQL CUSTOMER ERROR");
            return;
        }
        QMessageBox::about(this,"SUCCESS","you have pay for them")  ;
        ui->lineEdit_4->setText(QString::number(temp1,10,2));
        ui->lineEdit_4->repaint();
        ui->pushButton_2->hide();
        del_goods();
    }
    else
    {
        pay *a;
        a=new pay(this);
        a->show();
        connect(a,SIGNAL(havepay()),this,SLOT(del_goods()));
    }
}
void Client::SetCustomer(int id)
{
    QSqlQuery query(*db);
    QString temp="SELECT * FROM customer WHERE id=";
    temp+=QString::number(id,10);
    if(!query.exec(temp))
    {
        QMessageBox::warning(this,"ERROR","SELECT SQL CUSTOMER ERROR");
        return;
    }
    query.next();
    ui->lineEdit->setText(query.value("name").toString());
    ui->lineEdit_2->setText(QString::number(query.value("age").toInt(),10));
    ui->lineEdit_3->setText(query.value("male").toString());
    ui->lineEdit_4->setText(QString::number(query.value("money").toDouble(),10,2));
    ui->lineEdit->repaint();
    ui->lineEdit_2->repaint();
    ui->lineEdit_3->repaint();
    ui->lineEdit_4->repaint();
    nowMoney=query.value("money").toDouble();
    query.clear();
}
void Client::AddAGoods(int id)
{
    goodsID.append(id);
    QSqlQuery query(*db);
    QString temp="SELECT * FROM goods WHERE id=";
    temp+=QString::number(id,10);
    if(!query.exec(temp))
    {
        QMessageBox::warning(this,"ERROR","SELECT SQL GOODS ERROR");
        return;
    }
    query.next();
    temp="商品名:";
    temp+=query.value("name").toString();
    ui->textEdit->append(temp);
    temp="售价:";
    double money=query.value("price").toDouble();
    total+=money;
    ui->label_8->setText(QString::number(total,10,2));
    ui->label_8->repaint();
    temp+=QString::number(money,10,2);
    ui->textEdit->append(temp);
}
void Client::clear()
{
    total=0;
    ui->label_8->setText(QString::number(total,10,2));
    ui->label_8->repaint();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->textEdit->clear();
    customer=false;
    goodsID.clear();
}
void Client::del_goods()
{
    QSqlQuery query(*db);
    QString temp;
    QString temp1="DELETE FROM card WHERE id=";
    QString temp2="DELETE FROM goods WHERE id=";
    for(int i=0;i<goodsID.size();i++)
    {
        temp=temp1;
        temp+=QString::number(goodsID.at(i),10);
        if(!query.exec(temp))
        {
            QMessageBox::warning(this,"ERROR","DELETE SQL CARD ERROR");
            return;
        }
        query.clear();
        temp=temp2;
        temp+=QString::number(goodsID.at(i),10);
        if(!query.exec(temp))
        {
            QMessageBox::warning(this,"ERROR","DELETE SQL GOODS ERROR");
            return;
        }
    }
    clear();
}
void Client::on_pushButton_3_clicked()
{
    clear();
}
