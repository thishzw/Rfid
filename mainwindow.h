#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QByteArray>
#include<QSqlDatabase>
#include<QSql>
#include<QSqlDriver>
#include"login.h"
#include<QMessageBox>
#include<QException>
#include<QSqlRecord>
#include"EPCSDK.h"
#define HANDLE int
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
void setDataBase(QSqlDatabase *database);
void setCom(int no);
private slots:

void on_action_triggered();

void on_pushButton_clicked();

void on_pushButton_2_clicked();


void on_actionAdd_triggered();

void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    HANDLE com;
    bool add;
    bool isCustomer;
    bool identify;
    unsigned char EPC[24]  ;
    int cardID;
    void mode();
    int no;
};

#endif // MAINWINDOW_H
