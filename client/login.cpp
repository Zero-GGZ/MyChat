#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "common.h"
#include "QMessageBox"
//#include "QThread"

extern User global_user;

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{

    ui->setupUi(this);
    QPalette  pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    ui->login_message->setPalette(pe);
}

login::~login()
{
    delete ui;
}

void login::socket_Read_Data()
{
    QString str = global_user.socket->readAll();
    if(!str.isEmpty())
    {
        qDebug()<<str;
        //验证登录成功与否,服务器进行密码验证
        if(str=="login success")    //密码正确
        {
            disconnect(this->dis);
            global_user.is_login_succeed=true;
            this->close();      //关闭登录窗口
        }
        else
        {
            ui->login_message->setText("密码错误");
        }
    }
}
void login::on_login_Button_clicked()
{

    global_user.user_id=ui->user_id->text();
    global_user.passwd=ui->user_passwd->text();

    global_user.socket->connectToHost("106.13.106.106", 8089);    //连接服务器
//    global_user.socket->connectToHost("192.168.0.104", 8089);    //连接服务器
    if (global_user.socket->waitForConnected(200))
    {
         qDebug("Connected!");
         string message=to_string(sizeof(global_user.user_id.toStdString()))+global_user.user_id.toStdString()+"5"+"login"+
                 to_string(global_user.passwd.length())+global_user.passwd.toStdString();       //支持中文用户注册登录
         qDebug(message.c_str());
         global_user.socket->write(message.c_str(),message.length());
         qDebug("login successd!");
         this->dis=connect(global_user.socket, &QTcpSocket::readyRead, this, &login::socket_Read_Data);
    }
    else
    {
        global_user.is_login_succeed=false;
        qDebug("Connected erro");
        ui->login_message->setText("网络错误");
    }

}
