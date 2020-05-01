#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QMetaObject::Connection dis;
private slots:
    void on_register_Button_clicked();
    void socket_Read_Data();
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
