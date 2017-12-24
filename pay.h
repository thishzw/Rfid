#ifndef PAY_H
#define PAY_H

#include <QDialog>
#define HANDLE int
namespace Ui {
class pay;
}

class pay : public QDialog
{
    Q_OBJECT

public:
    explicit pay(QWidget *parent = 0);
    ~pay();
signals:
    void havepay();
    void notpay();
private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::pay *ui;
    bool state;
};

#endif // PAY_H
