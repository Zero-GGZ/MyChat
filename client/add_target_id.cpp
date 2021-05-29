#include "add_target_id.h"
#include "ui_add_target_id.h"
#include "common.h"
add_target_id::add_target_id(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_target_id)
{
    ui->setupUi(this);
}

add_target_id::~add_target_id()
{
    delete ui;
}


void add_target_id::on_add_id_clicked(QAbstractButton *button)
{
    if(button->text()=="OK"){

//        global_user.is_add_new_friend=
        global_user.new_id_name=ui->lineEdit->text();
//        configIniWrite->setValue("funds/total_funds",funds_count);
//        QChar temp ='0'+funds_count;
//        configIniWrite->setValue(QString("funds/")+temp,ui->lineEdit->text());
//        delete configIniWrite;
        qDebug("add new id success");
    }
}
