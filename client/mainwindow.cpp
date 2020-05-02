#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"

extern User global_user;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(global_user.socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    ui->user_id_number->setText(global_user.user_id);
//    ui->list_friend_id        //初始化用户列表
}

MainWindow::~MainWindow()
{
    delete ui;
}


//TCP接收函数
void MainWindow::socket_Read_Data()
{
    QString str = global_user.socket->readAll();
    qDebug()<<str;

    if(!str.isEmpty())
    {
        ui->ReceivetextBrowser->append(str);

    }
}

void MainWindow::on_pushButton_clicked()    //消息发送按钮
{
    QString input=ui->inputbox->toPlainText();
    global_user.friend_id=ui->tmp_friend_id->text();
    qDebug()<<global_user.user_id;
    qDebug()<<input;
    if(input.length()>0)
    {
//        QString message=global_user.user_id+':'+global_user.friend_id+':'+input;
        string input_message=to_string(global_user.user_id.toStdString().size())+global_user.user_id.toStdString()+"4"+"chat"+
                to_string(global_user.friend_id.toStdString().size())+global_user.friend_id.toStdString()+input.toStdString();
//        string input_message=message.toStdString();
        global_user.socket->write(input_message.c_str(),input_message.length());
        ui->inputbox->clear();

        ui->ReceivetextBrowser->append(global_user.user_id+':'+input);
        qDebug(input_message.c_str());

    }
}

void MainWindow::on_list_friend_number_clicked(const QModelIndex &index)
{
    global_user.friend_id=ui->list_friend_number->currentItem()->text();
    ui->friend_id_number->setText(global_user.friend_id);
}
