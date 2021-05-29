#ifndef COMMON_H
#define COMMON_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <iostream>

enum OPERATION_ID{      //客户端数据报文类型
    ID_Login = '0',
    ID_Register,
    ID_Chat_user,
    ID_Chat_group,
    ID_Inguiry,     //询问式接受消息
    ID_Logout,
    ID_Add_friend,
    ID_Add_group,

};

class User
{
public:
    User() {}
//    QString user_id;
    QString user_name;
    QString passwd;
    QString target_id;
//    QString target_id;
    int operation;
//    QString messege;
    QString new_id_name;
    QTcpSocket *socket=new QTcpSocket();

    QString ctreat_json_data(); //登录+注册使用
    QString ctreat_json_data(QString data); //聊天使用
    QString ctreat_json_data(QString message,QString operation);
    QString ctreat_json_data(int add_new_target_id  );   //加好友或者群
    QString get_data_from_json(QString json);
    bool is_login_succeed =false;
    bool is_update_friend_list=false;
//    bool is_add_new_friend=false;
//    bool is_add_new_group=false;
};

extern User global_user;

#endif // COMMON_H
