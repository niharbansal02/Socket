//Client Side
{
    cout<<"\n Welcome to quiz game";
    cout<<"\n What is 2+2 "<<endl;
    cout<<" a. 4\t b.6\n c. 8\t d. 10";
    char ch;
    cin>>ch;
    ch=tolower(ch);
    send(sock,ch,1,0);
    

}