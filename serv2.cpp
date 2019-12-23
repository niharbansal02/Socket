//Modfication
//Working fine
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<string>
#include<string.h>
#include<arpa/inet.h>

using namespace std;

int main()
{
    // Create a socket
    int listening=socket(AF_INET,SOCK_STREAM,0);                 //AF_INET is IPv4    
    if(listening==-1)
    {
        cerr<<" Can't create socket.";                          //cerr used if incase output is printed to a file, this doesnot get into that!
        return -1;
    }

    // Bind the socket to an IP/Port
    sockaddr_in hint;
    hint.sin_family=AF_INET;
    hint.sin_port=htons(54000);
    inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);
    
    if(bind(listening,(sockaddr*)&hint,sizeof(hint))==-1)              
    {
        cerr<<" Can't bind to IP/Port";
        return -2;
    }

    // Mark socket for Listening in 
    if(listen(listening,SOMAXCONN)==-1)
    {
        cerr<<" Can't Listen!";
        return -3;
    }

    // Accept a call
    sockaddr_in client;
    socklen_t clientSize=sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket=accept(listening,(sockaddr*)&client,&clientSize);
    if(clientSocket==-1)
    {
        cerr<<" Problem with client connecting!";
        return -4;
    }

    // Close the listening socket
    close(listening);
    memset(host,0,NI_MAXHOST);                                      //Cleanup of host
    memset(svc,0,NI_MAXSERV);                                       //Cleanup of svc
    
    int result=getnameinfo((sockaddr*)&client,clientSize,host,NI_MAXHOST,svc,NI_MAXSERV,0);
    if(result)                                                      //result==0
    {
        cout<<host<<" connected on "<<svc<<endl;
    }
    else
    {
        inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
        cout<<host<<" connected on "<<ntohs(client.sin_port)<<endl;
    }
    close(listening);
    // While recieving, display & echo message
    char buf[4069];
       
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
        cout<<" Recieved: "<<string(buf,0,bytesRecv)<<endl;               // Changed string to (char*)&
        
        //Resend Message    
        send(clientSocket,buf,bytesRecv+1,0);
    
    }

    // close socket
    close(clientSocket);

    return 0;
}


