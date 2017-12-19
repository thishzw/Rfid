#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSql>
#include<QSqlDriver>
#include<QSqlQuery>
#include"mainwindow.h"
#include"client.h"
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    void setDataBase(QSqlDatabase *);
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase *db;
};

#endif // LOGIN_H
