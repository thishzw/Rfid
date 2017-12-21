#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include<QSqlDatabase>
#include<QSql>
#include<QSqlDriver>
#include<QSqlQuery>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include"reader.h"
#include<QMessageBox>
#include<QVector>
namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();
void setDataBase(QSqlDatabase *database);
void setCom(Reader *comReader);
private slots:
void on_pushButton_clicked();

void on_pushButton_2_clicked();
void clear();
void on_pushButton_3_clicked();

private:
    Ui::Client *ui;
    QSqlDatabase *db;
    Reader *com;
    QVector<int> goodsID;
    char cardNum;
    char EPC[12];
    bool customer;
    int customerID;
    double total;
    double nowMoney;
void SetCustomer(int id);
void AddAGoods(int id);
void del_goods();
};

#endif // CLIENT_H
