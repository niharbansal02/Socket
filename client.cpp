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
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        cerr<<" Can't create socket";
        return 1;
    }

    // Create a hint struct. for the server we're connecting with
    int port=54000;
    string ipAddress="127.0.0.1";

    sockaddr_in hint;
    hint.sin_family=AF_INET;
    hint.sin_port=htons(port);
    inet_pton(AF_INET,ipAddress.c_str(),&hint.sin_addr);

    // connect the server on the socket
    int connectRes=connect(sock,(sockaddr*)&hint,sizeof(hint));
    if(connectRes==-1)
    {
        cerr<<" Connection failed";
        return 1;
    }

    // while loop
    char buf[4096];
    string userInput;

    while(true)
    {

        // Enter lines of text
        cout<<"> ";
        getline(cin,userInput);

        // sent to server
        int sendRes=send(sock,userInput.c_str(),sizeof(userInput)+1,0);             //+1 because we send the /0 also
        if(sendRes==-1)
        {
            cerr<<" Coudnot connect to server ";
            continue;                                                               //It will repeat the loop from start
        }

        // wait for response
        memset(buf,0,4096);
        int bytesRecieved=recv(sock,buf,4096,0);

        // display response
        cout<<" Server> "<<string(buf,bytesRecieved)<<" \r\n";    
    }
    // close the socket
    close(sock);


    return 0;
}