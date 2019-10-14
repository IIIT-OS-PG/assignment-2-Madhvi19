#include<iostream>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 8080

using namespace std;

int main(int argc, char const *argv[])
{
	int sock=0, valread; 
    char *path="/home/madhvi/2019201061_Q3.cpp"; 
    
 	struct sockaddr_in serv_addr; 
    sock=socket(AF_INET, SOCK_STREAM, 0); 
    if (sock<0) 
    { 
        cout<<"Socket creation error"; 
        return -1; 
    } 
   
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_port=htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

  	FILE *fp=fopen(path, "rb");
  	if(fp==NULL)
  	{
  		perror("File not found");
  	}
  	fseek (fp, 0, SEEK_END);
  	int size=ftell(fp); 
 	rewind (fp);

 	char buffer[1024]={'\0'};

 	//bzero(buffer, 1024);
 	int block;

 	while((block=fread(buffer, sizeof(char), 1024, fp))>0 && size>0)
 	{
		
		if(send(sock, buffer, block, 0)<0)
		{
			perror("Failed to send the file");
			break;
		}
		bzero(buffer, 1024);
		size = size-block;
	}
	fclose(fp);
	char buff[1024];
    valread=read(sock, buff, 1024); 
   
    cout<<buff;
    cout<<endl;
    cout<<"TATA";
    close(sock);



	return 0;
}