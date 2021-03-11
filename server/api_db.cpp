#define EXPAND_MY_SSQLS_STATICS 
#include "api_db.h"


// #include <iostream>
// #include <string>
// #define DB_NAME "mychat"
// #define SERVER  "localhost"
// #define USER    "root"
// #define PASSWD  "123"

// using namespace mysqlpp;
// using namespace std;
mysqlpp::Connection conn(false);
bool db_connect(){
    // const char* db = 0, *server = 0, *user_q = 0, *password = "";
	// db = "mychat";
	// server = "localhost";
	// user_q = "root";
	// password = "123";

    if(conn.connect(DB_NAME, SERVER, USER, PASSWD))
    {
        cout << "connect success"<<endl;
        return true;
    }
    else
        cout<< "connect error"<<endl;
    return false;
}

//获取昵称、密码、好友列表
User db_get_data(string name){
	if(name.empty())
		return User();
	mysqlpp::Query query = conn.query("SELECT * FROM user where name=\""+name+'\"');
	cout <<query<<endl;
	mysqlpp::StoreQueryResult res = query.store();
    if (!res) 
    {
        cout << "read data from database error"<<endl;
        return User ();
    }
    User row=res[0]; //取一行数据
    return row;
} 

//写入数据，如名字、ID、密码、好友列表
bool db_write_data(int id,string name,string passwd,string friend_id){
    User row;
    row.id=id;
    row.name=name;
    row.passwd=passwd;
    row.friend_id=friend_id;
    mysqlpp::Query query = conn.query();
    query.insert(row);
    cout << "Query: " << query << endl;
    query.execute( );
}       

//更新用户好友数据
bool db_write_data(string name,string new_data){
    mysqlpp::Query query = conn.query("SELECT * FROM user where name=\""+name+'\"');
    mysqlpp::StoreQueryResult res = query.store();
    if (!res) 
    {
        cout << "read data from database error"<<endl;
        return false;
    }
    User row=res[0]; //取一行数据
    User orgin_row=row;
    row.friend_id +=new_data;
  
    query.update(orgin_row,row);
    query.execute( );
    return true;
}  
//更新用户客户端连接id数据
bool db_write_data(string name,int client_id){
    db_connect();
    mysqlpp::Query query = conn.query("SELECT * FROM user where name=\""+name+'\"');
    mysqlpp::StoreQueryResult res = query.store();
    if (!res) 
    {
        cout << "read data from database error"<<endl;
        return false;
    }
    User row=res[0]; //取一行数据
    User orgin_row=row;
    row.client_id =client_id;
    cout <<client_id<<endl;
    query.update(orgin_row,row);
    query.execute( );
    return true;
}  
