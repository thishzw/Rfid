#include "login.h"
#include "ui_login.h"
#include<QDebug>
#include<QMessageBox>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setMaximumHeight(312);
    this->setMaximumWidth(252);
    this->setMinimumHeight(312);
    this->setMinimumHeight(252);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_2_clicked()
{
    QString user=ui->lineEdit->text();
    QString pass=ui->lineEdit_2->text();
    QSqlQuery query(*db);
    query.exec("SELECT * FROM login WHERE UserName=\'"+user+"\' AND PassWord=\'"+pass+"\'");
    if(query.first())
    {
        this->hide();
        MainWindow *server;
        server=new MainWindow(this);
        server->setCom(ui->spinBox->text().toInt());
        server->setDataBase(db);
        server->show();
    }
    else
    {
        QMessageBox::warning(this,"ERROR","The Password is wrong,please check it again!",QMessageBox::Ok,QMessageBox::Cancel);
    }
}
void Login::setDataBase(QSqlDatabase * database)
{
    db=database;
}

void Login::on_pushButton_clicked()
{
    this->hide();
    Client *client;
    client=new Client(this);
    client->setDataBase(db);
    client->setCom(ui->spinBox->text().toInt());
    client->show();
}

void Login::on_pushButton_3_clicked()
{
    close();
}
