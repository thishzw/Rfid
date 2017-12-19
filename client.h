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
void setCom(int com);
private slots:
void on_pushButton_clicked();

void on_pushButton_2_clicked();

private:
    Ui::Client *ui;
    QSqlDatabase *db;
    Reader *com;
    char cardNum;
    char EPCs[100][12];
    char TIDs[100][12];
    bool customer;
    int customerID;
    double total;
    double nowMoney;
void SetCustomer(int id);
void AddAGoods(int id);
void clear();
};

#endif // CLIENT_H
