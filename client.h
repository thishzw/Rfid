#ifndef CLIENT_H
#define CLIENT_H
#include"login.h"
#include <QMainWindow>
#include<QSqlDatabase>
#include<QSql>
#include<QSqlDriver>
#include<QSqlQuery>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QMessageBox>
#include<QVector>
#include"EPCSDK.h"
#define HANDLE int
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
void setCom(int no);
private slots:
void on_pushButton_clicked();

void on_pushButton_2_clicked();
void clear();
void on_pushButton_3_clicked();

private:
    Ui::Client *ui;
    QSqlDatabase *db;
    HANDLE com;
    QVector<int> goodsID;
    unsigned char cardNum;
    unsigned char EPC[12];
    bool customer;
    int customerID;
    double total;
    double nowMoney;
    int no;
void SetCustomer(int id);
void AddAGoods(int id);
void del_goods();
};

#endif // CLIENT_H
