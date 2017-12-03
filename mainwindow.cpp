#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readCom()
{

}

void MainWindow::on_pushButton_clicked()
{

}
