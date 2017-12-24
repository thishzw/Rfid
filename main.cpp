#include"login.h"
#include <QApplication>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSql>
#include<QSqlDriver>
#include<QLibrary>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase *db;
    db=new QSqlDatabase();
    (*db)= QSqlDatabase::addDatabase("QMYSQL");
    db->setDatabaseName("RFID");
    db->setUserName("RFIDUser");
    db->setPassword("RFID");
    db->setHostName("127.0.0.1");
    if(!db->open())
    {
        QMessageBox::warning(0,"DataBase ERROR","Check whether your mysql server is on and try again",QMessageBox::Ok,QMessageBox::Cancel);
    }
    Login w;
    w.setDataBase(db);
    w.show();

    return a.exec();
}
