//Working fine
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<string>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

using namespace std;
class splug
{
    int listening;
    sockaddr_in hint,client;
    socklen_t clientSize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    int clientSocket;
    int result;
    char buf[4096];
    int bytesRecv;
    string getString;

    public:
    splug()
    {
        listening=socket(AF_INET,SOCK_STREAM,0);
        if(listening==-1)
            cerr<<"\033[1;31m Can't create socket.\033[0m ";
    }
    
    int bind_socket()
    {
        hint.sin_family=AF_INET;
        hint.sin_port=htons(54000);
        inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);
        if(bind(listening,(sockaddr*)&hint,sizeof(hint))==-1)
        {
            cerr<<"\033[1;31m Can't bind to IP/PORT\033[0m";
            return -1;
        }
    }

    int listen_in()
    {
        if(listen(listening,SOMAXCONN)==-1)
        {
            cerr<<"\033[1;31m Can't listen!\033[0m";
            return -1;
        }

    }

    int accept_call()
    {
        clientSize=sizeof(client);
        clientSocket=accept(listening,(sockaddr*)&client,&clientSize);
        if(clientSocket==-1)
        {
            cerr<<"\033[1;31m Problem with client connecting!\033[0m";
            return -1;
        }
    }

    void close_listening()
    {
        close(listening);
        memset(host,0,NI_MAXHOST);                                      //Cleanup of host
        memset(svc,0,NI_MAXSERV);  
    }

    void connect_message()
    {
        result=getnameinfo((sockaddr*)&client,clientSize,host,NI_MAXHOST,svc,NI_MAXSERV,0);
        if(result)                                                      //result==0
        {
            cout<<"\033[1;36m"<<host<<"\033[0m connected on "<<"\033[1;33m"<<svc<<"\033[0m"<<endl;
        }
        else
        {
            inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
            cout<<"\033[1;36m"<<host<<"\033[0m connected on "<<"\033[1;33m"<<ntohs(client.sin_port)<<"\033[0m"<<endl;
        }
    }

    void data_to_client();

    ~splug()
    {
        close(clientSocket);
    }


};

int main()
{
    splug obj;
    if(obj.bind_socket()==-1)
        exit(0);
    if(obj.listen_in()==-1)
        exit(0);
    if(obj.accept_call()==-1)
        exit(0);
    obj.close_listening();
    obj.connect_message();
    obj.data_to_client();


    return 0;
}

void splug::data_to_client()
{
    while(true)
    {
        //Clear the buffer
        memset(buf,0,4096);

        // Wait for message
        int bytesRecv=recv(clientSocket,buf,4096,0);
        if(bytesRecv==-1)                                                       
        {
            cerr<<" There was a connection issue"<<endl;
            break;
        }
        else if(bytesRecv==0)
        {                   
            cout<<" The client disconnected"<<endl;
            break;
        }     
        
        //Displady message 
        cout<<"\033[1;32mRecieved: "<<string(buf,0,bytesRecv)<<"\033[0m"<<endl;               // Changed string to (char*)&
        
        //Resend Message
        // system("color 02");                                          //sh color command not found
        // cout<<"> ";
        // getline(cin,getString);
        // send(clientSocket,getString.c_str(),sizeof(getString)+1,0);
        // cout << "\033[1;31mbold red text\033[0m\n";
        send(clientSocket,buf,bytesRecv+1,0);
    }
}
