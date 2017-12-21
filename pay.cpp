#include "pay.h"
#include "ui_pay.h"
#include <QLabel>
#include <QMovie>
pay::pay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pay)
{
    ui->setupUi(this);
    QMovie *movie =new QMovie("./photo/1513836443.png");
    ui->label->setMovie(movie);
    movie->start();
}

pay::~pay()
{
    delete ui;
}


void pay::on_pushButton_clicked()
{
    emit havepay();
    close();
}

void pay::on_pushButton_2_clicked()
{
    emit notpay();
    close();
}
