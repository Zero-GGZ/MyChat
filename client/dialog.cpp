#include "dialog.h"
#include "ui_dialog.h"
#include "common.h"
#include "mainwindow.h"

extern User global_user;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QPalette  pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->register_message->setPalette(pe);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::socket_Read_Data()
{
    static bool flag=false;
    if(flag)
        return ;
    QString str = global_user.socket->readAll();
    if(!str.isEmpty())
    {
        qDebug()<<str;
        //验证注册成功与否,服务器进行密码验证
        if(str=="register success")    //密码正确
        {
            flag=true;
            disconnect(this->dis);
            ui->register_message->setText("注册成功");
        }
        else
        {
            ui->register_message->setText("注册失败");
        }
    }
}

void Dialog::on_register_Button_clicked()
{
    global_user.user_name=ui->user_id->text();
    global_user.passwd=ui->user_passwd->text();

    QString passwd_verify=ui->user_passw_verify->text();
    if(passwd_verify!=global_user.passwd)
    {
        ui->register_message->setText("密码不一致");
        return;
    }
//    global_user.socket->connectToHost("106.13.106.106", 8089);    //连接服务器
    global_user.socket->connectToHost("192.168.123.128", 8089);    //连接服务器
    if (global_user.socket->waitForConnected(200))
    {
         qDebug("Connected!");
         this->dis=connect(global_user.socket, &QTcpSocket::readyRead, this, &Dialog::socket_Read_Data);
         string message=to_string(global_user.user_name.toStdString().size())+global_user.user_name.toStdString()+"8"+"register"+
                 to_string(global_user.passwd.length())+global_user.passwd.toStdString();       //支持中文用户注册登录
         qDebug(message.c_str());
         global_user.socket->write(message.c_str(),message.length());
    }
    else
    {
        ui->register_message->setText("网络错误");
    }
}
