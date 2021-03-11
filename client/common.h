#ifndef COMMON_H
#define COMMON_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <iostream>


class User
{
public:
    User() {}
//    QString user_id;
    QString user_name;
    QString passwd;
    QString friend_id;
    QString operation;
//    QString messege;
    QTcpSocket *socket=new QTcpSocket();

    QString ctreat_json_data(); //登录+注册使用
    QString ctreat_json_data(QString data); //聊天使用
    QString get_data_from_json(QString json);
    bool is_login_succeed =false;
    bool is_update_friend_list=false;
};

extern User global_user;

#endif // COMMON_H
