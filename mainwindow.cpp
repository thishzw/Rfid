#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    com=new QSerialPort();
    com->setPortName("COM3");
    com->setBaudRate(QSerialPort::Baud9600);
    com->setDataBits(QSerialPort::Data8);
    com->setFlowControl(QSerialPort::NoFlowControl);
    com->setParity(QSerialPort::NoParity);
    com->setStopBits(QSerialPort::OneStop);
    com->close();
    if(com->open(QIODevice::ReadWrite))
    {
        connect(com,SIGNAL(readyRead()),this,SLOT(readCom()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readCom()
{
    QByteArray temp=com->read(1);

        while(!temp.isEmpty())
        {
            ui->textEdit->insertPlainText(temp.toHex());
            ui->textEdit->insertPlainText(" ");
            temp=com->read(1);
        }
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray temp;
    char a[5]={0XA0,0X03,0X82,0X00,0XDB};
    temp.append(a,5);
    bool s;
    s=com->write(temp);
    qDebug()<<temp;
}
