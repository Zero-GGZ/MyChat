#include <iostream>
#include <mysql++.h>
#include <json/json.h>
#include <string>
#include "api_db.h"

#include <iostream>
#include <string>
#include <vector>
#include "netinet/in.h"
#include <unistd.h>
#include "string.h"
#include "sys/types.h"
#include "sys/socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "api_json.h"
#include <thread>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/epoll.h> 
#include <map>
using namespace std;

#define PORT 8089
#define MAX_QUE_CONN_NM 50
#define BUFFER_SIZE 1024

struct sockaddr_in server_sockaddr,client_sockaddr;

int sockfd,client_fd,linsten_fd;


int epfd;
struct epoll_event events[100];
bool FLAGE=false;
map<string,int> client_info;

int init_server()   //绑定ip和端口等
{
    /*establish a socket connect*/
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
        cout << "socket establish faild"<<endl;
		exit(1);
	}
    /*config the pamater about sockaddr_in*/
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero),8);
	if(bind(sockfd,(struct sockaddr*)&server_sockaddr,
					sizeof(struct sockaddr)) == -1)
	{
        cout << "bind  faild"<<endl;
		exit(1);
	}	
	cout << "bind  success"<<endl;
	
	if((linsten_fd=listen(sockfd,MAX_QUE_CONN_NM)) == -1)
	{
        cout << "listen  faild"<<endl;
		exit(1);
	}
    cout << "Listening...."<<endl;
}  

enum OPERATION_ID{
	ID_Login = '0',
	ID_Register,
	ID_Chat,
	ID_Logout,
};



void tcp_send_data(int fd,string message)
{
    // cout <<message<<endl;
    // msgrcv(msgId,&readBuf,sizeof(readBuf.mtext),888,0);
    if(write(fd,message.c_str(),message.size())==-1)
        cout<< "tcp send data error"<<endl;
}



void process_data(Json::Value rev_data,int this_fd){
	char operation = rev_data["operation"].asString()[0];
	switch(operation){
		case ID_Login: {
            User data=db_get_data(rev_data["name"].asString()); 
            if(data.name.empty())
                cout <<"dont find user:"<<rev_data["name"].asString()<<endl;
            else if(data.passwd==rev_data["passwd"].asString()){
                tcp_send_data(client_fd,"login success");
                Json::Value W_root;
                W_root["operation"]="friend_data";
                // W_root["friend_id"]=rev_data["fiend_id"].asString();
                W_root["friend_id"]=data.friend_id;

                tcp_send_data(this_fd,write_json_to_string(W_root));
               
                client_info[data.name]=this_fd;

                // db_write_data(data.name,client_fd);
                cout <<"login success"<<endl;
            }
            else{
                tcp_send_data(this_fd,"passwd error");
                cout <<"passwd error"<<endl;
            }
            break;
        }

		case ID_Register:{
            db_write_data(0,rev_data["name"].asString(),rev_data["passwd"].asString(),"");
            cout <<"Register"<<endl;break;
        }
		case ID_Chat:{
            /*获取目标用户的fd号*/
            // User data=db_get_data(rev_data["friend_id"].asString());
            cout <<"Chat"<<endl;
            Json::Value W_root;
            W_root["name"]=rev_data["name"].asString();
            // W_root["friend_id"]=rev_data["fiend_id"].asString();
            W_root["message"]=rev_data["message"].asString();
            cout << rev_data["friend_id"].asString()<< ":" << client_info[rev_data["friend_id"].asString()]<<endl;
            tcp_send_data(client_info[rev_data["friend_id"].asString()],write_json_to_string(W_root));
            break;
        }
		case ID_Logout: cout <<"Logout"<<endl;break;
		default :break;
	}
}

void tcp_process_data_master()
{
    char buf_rev[1024];
     cout<<"tcp_process_data_master..."<<endl;
    while(1)
        while(FLAGE){
            cout<<"epoll_wait..."<<endl;
            int nfds = epoll_wait(epfd,events,100,10000);
            for(int i=0;i<nfds;++i){
                int fd =events[i].data.fd;
                int recvbytes = read(fd,buf_rev,BUFFER_SIZE);  //阻塞接收
                /*设定数据格式，user_ID+des_ID+数据*/
                if(recvbytes>0){
                    buf_rev[recvbytes]='\0';
                    process_data(read_json_from_string(buf_rev),fd);
                    //通过json提取数据
                }
            }	
        }
}
int main(){
	init_server(); //TCP 服务器开启
    db_connect();

    // char buf_rev[1024];
    int recvbytes;
    int sin_ssize;
    epfd = epoll_create(100);
    thread t(tcp_process_data_master);    //创建tcp数据接收线程 编译需要加上-std=c++11
	while(1){

		if((client_fd = accept(sockfd,		//阻塞等待客户端连接
                (struct sockaddr*)&client_sockaddr,(socklen_t*)&sin_ssize)) == -1)
        {
            cout << "accept  faild"<<endl;
            // continue;
        }
        //将clint_fd 写入epoll中
        struct epoll_event ev;
        ev.data.fd = client_fd;
        ev.events = EPOLLIN;
        epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);

        FLAGE=true;
       

	}
	
}