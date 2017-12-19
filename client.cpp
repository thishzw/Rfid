#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    total=0;
    ui->pushButton_2->hide();
}
void Client::setCom(int comd)
{
    com=new Reader();
    com->openCOMM(comd);
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
    clear();
    if(!com->IdentifyUploadedMultiTags(&cardNum,&EPCs[0][0],nullptr,0))
    {
        QMessageBox::warning(this,"ERROR","IdentifyUploadedMultiTags ERROR");
        return;
    }
    QSqlQuery query(*db);
    QString temp="SELECT * FROM card WHERE TID=\'";
    for(int i=0;i<cardNum;i++)
    {
        query.clear();
        QByteArray array;
        if(!com->ReadTIDByEpcID(EPCs[i],TIDs[i],0))
        {
            QMessageBox::warning(this,"ERROR","READ TID ERROR");
            return;
        }
        array.setRawData(TIDs[i],8);
        array=array.toHex();
        QString tid(array);
        QString temp1=temp+tid;
        temp1+="\'";
        if(!query.exec(temp1))
        {
            QMessageBox::warning(this,"ERROR","Identify SQL ERROR");
            return;
        }
        if(!query.size()>0)
            continue;
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
    }
    ui->pushButton_2->show();
}

void Client::on_pushButton_2_clicked()
{
    QSqlQuery query(*db);
    QString temp="UPDATE customer SET money=";
    double temp1=nowMoney-total;
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
      ui->pushButton_2->hide();
      clear();
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
    nowMoney=query.value("money").toDouble();
    query.clear();
}
void Client::AddAGoods(int id)
{
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
    temp+=QString::number(total,10,2);
    ui->textEdit->append(temp);
}
void Client::clear()
{
    total=0;
    ui->label_8->setText(QString::number(total,10,2));
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->textEdit->clear();
}
