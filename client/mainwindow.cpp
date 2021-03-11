#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include <algorithm>
extern User global_user;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(global_user.socket, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
    ui->user_id_number->setText(global_user.user_name);
//    ui->list_friend_id        //初始化用户列表
}

MainWindow::~MainWindow()
{
    delete ui;
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
            for(auto i:messege){
                if(i=='+'){
                    QListWidgetItem *item=new QListWidgetItem;
                    item->setText(data_char);
                    qDebug()<<"new friend";
                    ui->list_friend_number->addItem(item);
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
        QString input_message=global_user.ctreat_json_data(input);
        global_user.socket->write(input_message.toStdString().c_str(),input_message.toStdString().length());
        ui->inputbox->clear();

        ui->ReceivetextBrowser->append(global_user.user_name+':'+input);
        qDebug()<<input_message;

    }
}

void MainWindow::on_list_friend_number_clicked(const QModelIndex &index)
{
    global_user.friend_id=ui->list_friend_number->currentItem()->text();
    ui->friend_id_number->setText(global_user.friend_id);
}
