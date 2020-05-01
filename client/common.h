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
    QString user_id;
    QString passwd;
    QString friend_id;
    QTcpSocket *socket=new QTcpSocket();
    bool is_login_succeed =false;
};

extern User global_user;

#endif // COMMON_H
