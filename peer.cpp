#include<iostream>
#include<bits/stdc++.h>
#include<sys/socket.h>
#include<string.h>
#include<string>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<string>
#include<openssl/sha.h>
#include"getcommand.h"
//#include "newcom.h" 

#define TRACKER_PORT 8080
//#define chunk_size 1024*512

using namespace std;

char* po;
int port;


// ******************************client boundary ends here***********************************

void* connection_handler(void* sock)
{
	cout<<"inside connection_handler"<<endl;
	//char buffer[1024]={0};
	int newsock =*(int*)sock;
	char *path="/home/madhvi/OS/myfile";
	FILE *fp=fopen(path, "wb");
	if(fp==NULL)
	{
		perror("Cannot create file");
		cout<<"inside if"<<endl;
	}
	else
	{	
		cout<<"inside else"<<endl;
		//bzero(buffer, chunk_size);
		int block=0;

		char buffer[100]={'\0'};

    	block=read(newsock, buffer, 100);
    	cout<<buffer<<endl;

		cout<<"Done"<<endl;
			//fclose(fp);
	}
	
	char *msg="Got your message";
	send(newsock, msg, strlen(msg), 0);
		
	close(newsock);
}

void* server_func(void* xyz)
{
	cout<<"inside server thread"<<endl;

	struct sockaddr_in serv;

	int sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		perror("Socket failed!");
		exit(-1);
	}
	cout<<"after socket() function"<<endl;
	serv.sin_family=AF_INET;
	port=atoi(po);
	serv.sin_port=htons(port);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");
	cout<<"before bind"<<endl;

	if(bind(sockfd, (struct sockaddr *)&serv, sizeof(serv))<0)
	{
		perror("Cannot bind");
		exit(-1);
	}

	cout<<"after bind"<<endl;
	if(listen(sockfd, 4)<0)
	{
		perror("Cannot listen");
		exit(-1);
	}
	cout<<"waiting for client to connect"<<endl;
	int servlen=sizeof(serv);
	int newsock;
	pthread_t thread_ids[4];
	int no_of_clients=4;
	int x=0;
	while(no_of_clients)
	{
		if(newsock=accept(sockfd, (struct sockaddr *)&serv, (socklen_t*)&servlen))
			cout<<"connected"<<endl;

		if(pthread_create(&thread_ids[x] , NULL ,connection_handler,(void*)&newsock)<0)
        {
            perror("could not create thread");
            exit(-1);
        }
        no_of_clients--;
        x++;
        cout<<"client"<<x<<endl;
        cout<<"Handler assigned"<<endl;

	}
	for(int i=0;i<4;i++)
	{
		pthread_join(thread_ids[i], NULL);
	}
}

//*************************************server boundary ends*********************************************

int main(int argc, char *argv[])
{
	pthread_t th;
	po=argv[1];

	if(pthread_create(&th , NULL ,server_func,NULL)<0)
    {
        perror("could not create thread");
        exit(-1);
    }

    
    int sock=0, valread; 
   	cout<<"inside while"<<endl;
 	struct sockaddr_in serv_addr; 
	sock=socket(AF_INET, SOCK_STREAM, 0); 
  	if (sock<0) 
  	{	 
    	cout<<"Socket creation error";
    	exit(-1);  
  	} 
   
	serv_addr.sin_family=AF_INET; 
	serv_addr.sin_port=htons(TRACKER_PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
  	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
  	{ 


    	printf("\nInvalid address/ Address not supported \n");
    	exit(-1);  
  	} 
   
  	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) 
  	{
    	printf("\nConnection Failed \n"); 
    	exit(-1); 
  	} 
  	//cout<<"Socket address is "<<sock<<endl;
  	

  	while(1)
  	{
  		getcommand cm;
  		string input;
  		cout<<"What do you wanna do?"<<endl;
  		getline (cin, input);
  		cout<<"input command is "<<input<<endl;

  		bool stat=cm.command(input,sock,port);
  		if(!stat)
  			continue;

    }

	pthread_join(th, NULL);
	return 0;
}
