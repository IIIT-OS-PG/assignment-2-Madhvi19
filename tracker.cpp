#include<iostream>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<vector>
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

	//when a client uploads a file
	while(1)
	{	
		char buffer[1024];
		int valread=read(newsock, buffer, 1024); 
		string in(buffer);
		int len=in.length();
		char input[len+1];
		int i;
		for(i=0;i<len;i++)
		{
			input[i]=in[i];

		}
		input[i]='\0';
		vector<char*> v;
		char* temp=strtok(input, " ");
		while(temp!=NULL)
		{
			v.push_back(temp);
			temp=strtok(NULL," ");
		}
		for(int i=0;i<v.size();i++)
		{
			cout<<v[i]<<endl;
		}

		string upload="upload_file";
		if(upload==v[0])//upload_file
		{
			//calculate SHA1 of the file and send the SHA1 for each chunk 
			//to the tracker along with the user id and filename
			cout<<"Successful"<<endl;

		}
	}

	return 0;
}
