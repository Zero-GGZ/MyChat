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
#include <deque>
#include <list>

using namespace std;

#define PORT 8089
#define MAX_QUE_CONN_NM 50
#define BUFFER_SIZE 1024

#define DEBUG
#ifdef DEBUG
#define debug(str) cout<<str<<endl
#else
#define debug /\
/cout         //展开为注释
#endif


struct sockaddr_in server_sockaddr,client_sockaddr;

int sockfd,client_fd,linsten_fd;


int epfd;
struct epoll_event events[100];
bool FLAGE=false;
// map<string,int> client_info;

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



void tcp_send_data(int fd,string message){
    if(write(fd,message.c_str(),message.size())==-1)
        cout<< "tcp send data error"<<endl;
}

class Chat_user{
public:
    Chat_user(){}
    Chat_user(Chat_user& user){
        group_cash = user.group_cash;
    }
    inline void add_group_to_cash(string group_name){
        group_cash.push_back(make_pair(group_name,0));
    }
    void set_group_cash(string& groups){
        string group_name;
        for (auto i : groups){
            if(i=='+'){
                group_cash.push_back(make_pair(group_name,0));
                group_name.clear();
                continue;
            }
            group_name +=i;
        } 
    }
    vector<pair<string,long int>>& get_group_cash(){
        return group_cash;
    }
    virtual void add_message(string message){
        debug("user add message");
        message_queue.push_back(message);
    }
    virtual string get_message(){
        string temp = message_queue.front();
        message_queue.pop_front();
        return temp;
    }
    virtual bool message_empty(){
        return message_queue.empty();
    }
    Chat_user& operator=(const Chat_user&user){
        group_cash = user.group_cash;
        return *this;
    }
    ~Chat_user(){}
    
private:
    deque<string> message_queue;      //用户聊天信息缓冲栈，存储该用户的要接收的聊天信息
    vector<pair<string ,long int>> group_cash;          //缓存用户加入的群名
};

class Chat_group :public Chat_user{
public:
    Chat_group():cur_index(0){}
    bool message_empty(int index){
        return index >= cur_index;
    }
    virtual string get_message(int index){
        if(index < cur_index)
            return message_queue[message_queue.size()-(cur_index-index)];
        else
            return "";
    }
    virtual void add_message(string message){
        debug("group add message");
        message_queue.push_back(message);
        cur_index++;
    }
    long int get_index(){
        return cur_index;
    }

private:
    deque<string> message_queue;
    long int cur_index;
    // long int time;
};

map<string, Chat_user*> User_mp; //用户信息map


// void send_message(int fd,string message,string message){
//     read_json_from_string(message);
    
// }

void process_data(Json::Value rev_data, int this_fd){
	char operation = rev_data["operation"].asString()[0];
	switch(operation){
		case ID_Login: {
            User data=db_get_data(rev_data["name"].asString()); 
            if(data.name.empty())
                cout <<"dont find user:"<<rev_data["name"].asString()<<endl;
            else if(data.passwd==rev_data["passwd"].asString()){
                tcp_send_data(this_fd,"login success");
                Json::Value W_root;
                W_root["operation"]="friend_data";
                W_root["friend_id"] = data.friend_id;
                W_root["group_id"] = data.groups_id;
                

                Chat_user *user_obj=User_mp[rev_data["name"].asString()];
                if (user_obj == nullptr){
                    user_obj = new Chat_user;            //创建用户对象
                    User_mp.insert(make_pair(rev_data["name"].asString(), nullptr));
                    User_mp[rev_data["name"].asString()] = user_obj;
                }
                user_obj->set_group_cash(data.groups_id);
                debug(User_mp[rev_data["name"].asString()]);

                tcp_send_data(this_fd,write_json_to_string(W_root));
               // client_info[data.name]=this_fd;

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
        case ID_Chat_group:{}
		case ID_Chat_user:{
            /*获取目标用户的fd号*/
            // User data=db_get_data(rev_data["friend_id"].asString());
            cout <<"Chat"<<endl;
            Json::Value W_root;
            W_root["name"]=rev_data["name"].asString();
            // W_root["friend_id"]=rev_data["fiend_id"].asString();
            W_root["message"]=rev_data["message"].asString();
            cout << rev_data["friend_id"].asString()<<endl;

            Chat_user *user_obj=User_mp[rev_data["friend_id"].asString()];
            if (user_obj == nullptr){
                if(operation==ID_Chat_group)        //如果是群
                    user_obj = new Chat_group;
                else
                    user_obj = new Chat_user; //创建用户对象
                User_mp.insert(make_pair(rev_data["friend_id"].asString(), nullptr));
                User_mp[rev_data["friend_id"].asString()] = user_obj;
            }
            debug(User_mp[rev_data["friend_id"].asString()]);
            user_obj->add_message(write_json_to_string(W_root));
            // user_message_queue[rev_data["friend_id"].asString()].push_back(write_json_to_string(W_root));    //写入队列中

            // tcp_send_data(client_info[rev_data["friend_id"].asString()], write_json_to_string(W_root));
            break;
        }
        case ID_Inguiry:{
            debug("ID_Inguiry");

            Chat_user *user_obj = User_mp[rev_data["name"].asString()];
            // string user_name = rev_data["name"].asString();
            // auto &user_m_q = user_message_queue[user_name];
            string message;
            while (!user_obj->message_empty()){ //读取用户数据缓冲队列
                message = user_obj->get_message();
                debug("user_m_q:" << message);
                tcp_send_data(this_fd, message);
            }
            //读取用户群组，标记自己读取到的位置，时间戳加index_id
            // auto &user_g_c = user_group_cash[user_name];
            // for (auto& iter : user_g_c){
            //     while(group_message_queue[iter])
            // }
            Chat_group *group_obj = nullptr;
            auto& groups = user_obj->get_group_cash();
            string group_name;
            for (auto& iter : groups){
                debug(iter.first);
                if(User_mp[iter.first]!=nullptr){
                    group_obj = dynamic_cast<Chat_group *>(User_mp[iter.first]);
                    
                    int index = iter.second;
                    debug(iter.first<<"\t"<<index);
                    int cur_index = group_obj->get_index();

                    debug("group_message"<<index<<"\t"<<cur_index);
                    if (cur_index-index>10)
                        index = cur_index - 10;
                    while(index<cur_index){
                        message = group_obj->get_message(index++);
                        debug("user_m_q:" << message);
                        tcp_send_data(this_fd, message);
                    }
                    iter.second = cur_index;    //将群缓冲中消息接收缓冲的index同步
                }
            }
            break;
        }
        case ID_Add_friend:{
            db_update_groups(rev_data["name"].asString(), rev_data["target_id"].asString());    
            break;
        }
        case ID_Add_group:{
            db_update_groups(rev_data["name"].asString(), rev_data["target_id"].asString());
            auto userobj = User_mp[rev_data["name"].asString()];
            debug(rev_data["name"].asString());
            // for(auto&user:User_mp){
            //     cout << user.first << "\t" << user.second << endl;
            // }
            Chat_user *user_obj = User_mp[rev_data["name"].asString()];
            // cout << userobj << endl;
            userobj->add_group_to_cash(rev_data["target_id"].asString());
            break;
        }
        case ID_Logout: debug("logout");break;
		default :break;
	}
}

void tcp_process_data_master()
{
    char buf_rev[1024];
     cout<<"tcp_process_data_master..."<<endl;
    while(1)
        while(FLAGE){
            // cout<<"epoll_wait..."<<endl;
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
        ev.events = EPOLLIN;    //表示对应的文件描述符可以读
        epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);   //EPOLL_CTL_ADD 注册新的fd到epoll中

        FLAGE=true;

	}
}





