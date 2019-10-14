#include<iostream>
#include<bits/stdc++.h>
#include<openssl/sha.h>
#include<pthread.h>
#include<vector>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#define chunk_size 512*1024
using namespace std;


class getcommand
{
    public:
    int port_=0;
    void login_user(vector<char*> , int);
    void create_group(vector<char*>, int);
    void list_groups(vector<char*>, int);

    bool uploadfile(vector<char*> v, int sock)
    {
        //cout<<"inside uploadfile"<<endl;
          char* path=v[1];

        FILE *fp=fopen(path, "rb");
        if(fp==NULL)
        {
          perror("File not found");
          return false;
        }

        fseek (fp, 0, SEEK_END);
        int size=ftell(fp);
        vector<string> sha_container;
        rewind (fp);

        char buffer[chunk_size]={'\0'};

        //bzero(buffer, 1024);
        int block;

        while((block=fread(buffer, sizeof(char), chunk_size, fp))>0 && size>0)
        {
            unsigned char temp[SHA_DIGEST_LENGTH];
            char buf[SHA_DIGEST_LENGTH*2];

            memset(buf, 0x0, SHA_DIGEST_LENGTH*2);
            memset(temp, 0x0, SHA_DIGEST_LENGTH);

     
            SHA1((unsigned char *)buffer, strlen(buffer), temp);

            int i=0;
            for (i=0; i < SHA_DIGEST_LENGTH; i++)
            {
                sprintf((char*)&(buf[i*2]), "%02x", temp[i]);
            }
            //cout<<buf<<endl;
            buf[i]='\0';

            string ss=string(buf);
            string s=ss.substr(0,20);
           // cout<<s<<endl;

          
            sha_container.push_back(s);
          
            bzero(buffer, chunk_size);
            size=size-block;
        }

        fclose(fp);

        char result[10]={'\0'};
        int l=sha_container.size();
        sprintf(result, "%d", l);
        cout<<result<<" is number of shas"<<endl;

        send(sock,result,strlen(result),0);

        char b[10]={'\0'};
        read(sock,b,10);

        for(int i=0;i<sha_container.size();i++)
        {
            int length=sha_container[i].size();
          
            char cstr[length+1];
            bzero(cstr,length+1);

            strcpy(cstr, sha_container[i].c_str());
            cout<<cstr<<endl;

            int x;
            if((x=send(sock, cstr, length, 0))>0)
            {
                char buff[100]={'\0'};

                int valread=read(sock, buff, 100);
                cout<<buff<<endl;
            }
            else
            {
                perror("Cannot send SHA to the tracker\n");
            }
        }

    }

    void create_user(vector<char*> v, int sock)
    {	cout<<v[1]<<"user id"<<endl;
    	cout<<v[2]<<"password"<<endl;
    	cout<<"inside create user function"<<endl;

    	if(send(sock,v[1],strlen(v[1]),0)<0)
    	{
    		perror("Cannot send user id to the tracker\n");
    	}
    	char buf[100]={'\0'};
    	read(sock,buf,100);
        cout<<"ack of user id"<<endl;
    	cout<<buf<<endl;
    	if(send(sock,v[2],strlen(v[2]),0)<0)
    	{
    		perror("Cannot send user id to the tracker\n");
    	}
        cout<<"about to create user"<<endl;
    	bzero(buf,100);
    	read(sock,buf,100);
    	cout<<buf<<endl;
    	
    }



    int sort_command(vector<char*> v, int sock)
    {
      	cout<<"inside sort_command"<<endl;
      	
      	string createuser="create_user";
      	string upload="upload_file";
      	string login="login";
      	string creategroup="create_group";
      	string joingroup="join_group";
      	string leavegroup="leave_group";
      	string listrequests="list_requests";
      	string acceptrequest="accept_request";
      	string listgroups="list_groups";
      	string listfiles="list_files";
      	string download="download_file";
      	string logout="logout";
      	string showdownloads="show_downloads";
      	string stop="stop_sharing";

       	int status;
        if(v[0]==listgroups)
        {
            cout<<"inside list groups wala if"<<endl;
            char com[listgroups.size()+1];
            strcpy(com,listgroups.c_str());
            int x;
            cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)     //command sent
            {
                perror("Cannot send command to the tracker\n");
            }

            char buf[10]={'\0'};
            read(sock,buf,10);  //reading okay

            status =8;
            return status;

        }
      	if(v[0]==createuser)
      	{	
      		cout<<"inside create user wala if"<<endl;
      		char com[createuser.size()+1];
      		strcpy(com,createuser.c_str());
      		int x;
      		//cout<<com<<endl;
      		if ((x=send(sock,com,strlen(com),0))<0)
      		{
      			perror("Cannot send command to the tracker\n");
      		}
            char buf[10]={'\0'};
            read(sock,buf,10);
            //cout<<buf<<endl;

        	status=1;
        	return status;
      	}

        if(v[0]==upload)
        {   
            char com[upload.size()+1];
            strcpy(com,upload.c_str());
            int x;
            cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)
            {
                perror("Cannot send command to the tracker\n");
            }
            char buf[10]={'\0'};
            read(sock,buf,10);
            cout<<buf<<endl;

            status=14;
            return status;

        }
        if(v[0]==login)
        {
            cout<<"inside login user wala if"<<endl;
            char com[login.size()+1];
            strcpy(com,login.c_str());
            int x;
            cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)
            {
                perror("Cannot send command to the tracker\n");
            }
            char buf[10]={'\0'};
            read(sock,buf,10);
            cout<<buf<<endl;

            status=2;
            return status;
        }

        if(v[0]==creategroup)
        {
            cout<<"inside login user wala if"<<endl;
            char com[creategroup.size()+1];
            strcpy(com,creategroup.c_str());
            int x;
            //cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)
            {
                perror("Cannot send command to the tracker\n");
            }
            char buf[10]={'\0'};
            read(sock,buf,10);
            cout<<buf<<endl;

            status=3;
            return status;
        }
        else
        {
            return 0;
        }
    }

    bool command(string input, int sock,int port)
    {
        port_=port;
    	cout<<"inside command"<<endl;
    	char in[input.size() + 1];
      	strcpy(in, input.c_str());

      	vector<char*> v;
      	char* temp=strtok(in, " ");
      	while(temp!=NULL)
      	{
        	v.push_back(temp);
        	temp=strtok(NULL," ");
      	}
        for(int i=0;i<v.size();i++)
        {
            cout<<v[i]<<endl;
        }
        cout<<"calling sort command"<<endl<<endl;

      	int status=sort_command(v, sock);

      	if(status==0)
      	{
        cout<<"command not found"<<endl;
        return true;
      	}

      	if(status==1)
        {
            cout<<"calling create_user "<<endl;
      		create_user(v,sock);
            return true;
        }
        if(status==2)
        {
            login_user(v,sock);
            return true;
        }
        if(status==3)
        {
            create_group(v,sock);
        }

        if(status==8)
        {
            list_groups(v,sock);
        }
      		
      	if(status==14)
        {
      		bool x=uploadfile(v,sock);
      		if(!x)
      		{
      			return false;
      		}
      
        }
    }
    	
};


void getcommand::login_user(vector<char*> v, int sock)
{  
    //cout<<"inside login_user"<<endl;
    if(send(sock,v[1],strlen(v[1]),0)<0)
    {
      perror("Cannot send user id to the tracker\n");
    }
    char buf[100]={'\0'};
    read(sock,buf,100);
    //cout<<buf<<endl;
    if(send(sock,v[2],strlen(v[2]),0)<0)
    {
      perror("Cannot send user id to the tracker\n");
    }

    bzero(buf,100);
    read(sock,buf,100);
    //cout<<buf<<endl;
    string b=string(buf);
    string welcome="welcome!";
    string wrong="Wrong password";

    if(b==welcome)
    { 
      char port_ad[10];
      sprintf(port_ad, "%d", port_);
      //cout<<port_ad<<endl; 
  
      if(send(sock,port_ad,strlen(port_ad),0)<0)
      {
        perror("Cannot send socket address to the client\n");
      }
      bzero(buf,0);
      read(sock,buf,100);
      cout<<"Welcome!"<<endl;
    }
    else if(wrong==b)
    {
      cout<<"Wrong password\n";
    }
    else
    {
      cout<<"User does not exist\n";
    }
  

}

void getcommand::create_group(vector<char*> v, int sock)
{
    if(send(sock,v[1],strlen(v[1]),0)<0)
      perror("Error in sending\n");

    char buf[100]={'\0'};
    read(sock,buf, 100);
    cout<<buf;

    char port_ad[10];
    sprintf(port_ad, "%d", port_);
    cout<<"port of client is"<<port_ad<<endl;
    send(sock,port_ad, strlen(port_ad),0);
    bzero(buf,'\0');
    read(sock,buf,100);
    cout<<buf<<endl;
}

void getcommand::list_groups(vector<char*> v, int sock)
{
    send(sock,"hello",strlen("hello"),0);
    char len[10]={'\0'};
    read(sock,len,10);
    int length=atoi(len);

    send(sock,"ok",strlen("ok"),0);

    while(length--)
    {   
        char buffer[100]={'\0'};
        read(sock,buffer,100);
        cout<<buffer<<endl;
        send(sock,"okay",strlen("okay"),0);
    }

}