#ifndef TRANS_INCLUDED
#define TRANS_INCLUDED
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
using namespace std;
class Trans{
private:
    struct sockaddr_in servaddr,cliaddr;
    int servfd,clifd,length,serv_port;
    string serv_ip;
public:
    void sendMsg(bool *b,int len){
        cout<<"请输入另一台电脑的IP:";
        cin>>serv_ip;
        cout<<"请输入另一台电脑的端口:";
        cin>>serv_port;
        char *buf = new char[len];
        cout<<"发送信息: ";
        for(int i=1; i<=len; ++i){
            if(b[i]) buf[i-1]='1';
            else     buf[i-1]='0';
            cout<<buf[i-1];
        }
        cout<<endl;
        if ((servfd = socket(AF_INET,SOCK_STREAM, 0)) < 0){
            cout<<"socket生成失败!\n";
            exit(1);
        }
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family  = AF_INET;
        servaddr.sin_addr.s_addr  =  inet_addr(serv_ip.c_str());
        servaddr.sin_port  =  htons(serv_port);
        if(connect(servfd,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
           cout<<"无法连接!\n ";
            exit( 1 );
        }
        send(servfd, buf,len,0);
        cout<<"成功发送！"<<endl;
        close(servfd);
    }

    int recvMsg(char* temp){
        //char temp[1024]={'\0'};
        cout<<"请输入想监听的端口:";
        cin>>serv_port;
        if((servfd  =  socket(AF_INET,SOCK_STREAM, 0)) < 0){
              cout<<"socket生成失败!\n";
              exit(1);
        }
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family  =  AF_INET; // tcp
        servaddr.sin_port  =  htons(serv_port);
        servaddr.sin_addr.s_addr  =  htons(INADDR_ANY); // INADDR_ANY 0.0.0.0
        if(::bind(servfd,(struct sockaddr*)&servaddr, sizeof(servaddr))<0){
              cout<<"绑定失败!\n";
              exit(1);
        }
        if(listen(servfd,10) < 0){
              printf("监听失败!\n" );
              exit(1);
        }
        socklen_t len = sizeof (cliaddr);
        clifd = accept(servfd,(struct sockaddr*)&cliaddr, &len);
        if(clifd < 0){
            cout<<"接收失败!\n";
            exit(-1);
        }
        recv(clifd, temp,100,0);
        length = strlen(temp);

        return length;
    }

};



#endif // TRANS_INCLUDED
