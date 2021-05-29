#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add_target_id.h"
#include "common.h"
#include <algorithm>
#include <QTimer>
extern User global_user;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(global_user.socket, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
    ui->user_id_number->setText(global_user.user_name);
//    ui->list_friend_id        //初始化用户列表
    this->tim=new QTimer;
    tim->setInterval(1000);
    connect(tim,SIGNAL(timeout()),this,SLOT(on_timeout()));
    tim->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeout(){
//    qDebug()<<"timeout";
    QString input_message=global_user.ctreat_json_data("",QString(ID_Inguiry));    //询问数据
    global_user.socket->write(input_message.toStdString().c_str(),input_message.toStdString().length());

}


//TCP接收函数
void MainWindow::socket_Read_Data()
{

    QString str = QString::fromLocal8Bit(global_user.socket->readAll());
    qDebug()<<str;
    QString messege = global_user.get_data_from_json(str);

    if(!messege.isEmpty()){
        qDebug()<<messege;
        if(global_user.is_update_friend_list){  //更新好友列表
            QString data_char;
            int len=0;
            int index = messege.length();
            for(auto i:messege){
                index++;

                if(i=='+'){
                    if(index>=messege.length()){

                        len = data_char.toInt();
                        index=0;
                        data_char.clear();
                        continue;
                    }
                    QListWidgetItem *item=new QListWidgetItem;
                    item->setText(data_char);
                    if(index<=len)
                        ui->list_friend_number->addItem(item);
                    else
                        ui->list_group_number->addItem(item);

                    data_char.clear();
                    continue;
                }
                data_char +=i;

            }
            global_user.is_update_friend_list=false;
        }
        else
            ui->ReceivetextBrowser->append(messege);
    }
    else
        qDebug()<<"messege error";
}

void MainWindow::on_pushButton_clicked()    //消息发送按钮
{
    QString input=ui->inputbox->toPlainText();
    //    global_user.friend_id=ui->tmp_friend_id->text();
    qDebug()<<global_user.user_name;
    qDebug()<<input;
    if(input.length()>0)
    {
//        if()
        QString input_message=global_user.ctreat_json_data(input,QString(global_user.operation));  //群消息

        global_user.socket->write(input_message.toStdString().c_str(),input_message.toStdString().length());
        ui->inputbox->clear();

//        ui->ReceivetextBrowser->append(global_user.user_name+':'+input);
        qDebug()<<input_message;

    }


}

void MainWindow::on_list_friend_number_clicked(const QModelIndex &index)
{
    global_user.target_id=ui->list_friend_number->currentItem()->text();
    global_user.operation=ID_Chat_user;
    ui->target_id_number->setText(global_user.target_id);
}

void MainWindow::on_list_group_number_clicked(const QModelIndex &index)
{
    global_user.target_id=ui->list_group_number->currentItem()->text();
    global_user.operation=ID_Chat_group;
    ui->target_id_number->setText(global_user.target_id);
}


void MainWindow::on_add_friend_clicked()
{
    add_target_id w;
    w.setWindowTitle("添加好友/群");
    w.exec();
    //tcp发送数据给服务端
    QString input_message=global_user.ctreat_json_data(ID_Add_friend);  //群消息
    global_user.socket->write(input_message.toStdString().c_str(),input_message.toStdString().length());
}



void MainWindow::on_add_group_clicked()
{
    add_target_id w;
    w.setWindowTitle("添加好友/群");
    w.exec();
    QString input_message=global_user.ctreat_json_data(ID_Add_group);  //群消息
    global_user.socket->write(input_message.toStdString().c_str(),input_message.toStdString().length());
}


