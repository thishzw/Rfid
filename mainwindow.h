#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"reader.h"
#include<QByteArray>
#include<QSqlDatabase>
#include<QSql>
#include<QSqlDriver>
#include"login.h"
#include<QMessageBox>
#include<QException>
#include<QSqlRecord>
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
void setCom(Reader *com);
private slots:

void on_action_triggered();

void on_pushButton_clicked();

void on_pushButton_2_clicked();


void on_actionAdd_triggered();

void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Reader *comReader;
    QSqlDatabase *db;
    bool add;
    bool isCustomer;
    bool identify;
    char EPC[24]  ;
    int cardID;
    void mode();
    void WriteCustomer();
    void WriteGoods();
};

#endif // MAINWINDOW_H
