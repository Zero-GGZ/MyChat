#ifndef API_DB_H
#define API_DB_H

#if !defined (EXPAND_MY_SSQLS_STATICS)
#define MYSQLPP_SSQLS_NO_STATICS 
#endif 

#include <mysql++.h>
#include <iostream>
#include <string>
#include <ssqls.h>


sql_create_5(User,
1, 5,
mysqlpp::sql_int, id,
mysqlpp::sql_varchar, name,
mysqlpp::sql_varchar, passwd,
mysqlpp::sql_varchar, friend_id,
mysqlpp::sql_int,client_id
)

using namespace std;

#define DB_NAME "mychat"
#define SERVER  "localhost"
#define USER    "root"
#define PASSWD  "123"


bool db_connect();
User db_get_data(string name);     //获取昵称、密码、好友列表
bool db_write_data(int id,string name,string passwd,string friend_id);       //写入数据，如名字、ID、密码、好友列表
bool db_write_data(string name,int client_id);
#endif

