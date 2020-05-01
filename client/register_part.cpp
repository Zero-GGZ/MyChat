#include "register_part.h"
#include "ui_register_part.h"

p_regis::p_regis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::p_regis)
{
    ui->setupUi(this);
}

p_regis::~p_regis()
{
    delete ui;
}
