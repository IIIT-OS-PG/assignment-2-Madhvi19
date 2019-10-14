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
	struct sockaddr_in serv;

	int sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		perror("Socket failed!");
		exit(-1);
	}

	serv.sin_family=AF_INET;
	serv.sin_port=htons(PORT);
	serv.sin_addr.s_addr=inet_addr("127.0.0.1");

	if(bind(sockfd, (struct sockaddr *)&serv, sizeof(serv))<0)
	{
		perror("Cannot bind");
		exit(-1);
	}

	if(listen(sockfd, 10)<0)
	{
		perror("Cannot listen");
		exit(-1);
	}

	int servlen=sizeof(serv);
	int newsock;
	if((newsock=accept(sockfd, (struct sockaddr *)&serv, (socklen_t*)&servlen))<0)
	{
		perror("Cannot accept request");
		exit(-1);
	}

	//receive file froom client

	char buffer[1024]={0};
	char *path="/home/madhvi/OS/myfile.cpp";
	FILE *fp=fopen(path, "wb");
	if(fp==NULL)
	{
		perror("Cannot create file");
	}
	else
	{	
		bzero(buffer, 1024);
		int block=0;
		while((block=recv(newsock, buffer, 1024, 0))>0)
		{
			if(block<0)
			{
				perror("Error in receiving file");
			}
			int write_byte=fwrite(buffer, sizeof(char), block, fp);
			if(write_byte<block)
			{
				perror("File write failed");
			}
			else if(block)
			{
				break;
			}
			bzero(buffer, 1024);
		}

		cout<<"Done";
		fclose(fp);
	}
	
	char *msg="Meko file receive ho gyi hai";
	send(newsock, msg, strlen(msg), 0);
	close(newsock);

	return 0;
}
