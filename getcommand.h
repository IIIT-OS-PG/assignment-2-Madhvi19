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
    void leave_group(vector<char*>, int);
    void join_group(vector<char*>, int);
    void list_requests(vector<char*>, int);

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
        cout<<endl;
        read(sock,b,10);
        cout<<endl;
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
                cout<<endl;
                int valread=read(sock, buff, 100);
                cout<<endl;
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
    	char cru[100]={'\0'};
        cout<<endl;
        read(sock,cru,100);
        cout<<endl;

        cout<<"ack of user id"<<endl;
    	cout<<cru<<endl;
    	if(send(sock,v[2],strlen(v[2]),0)<0)
    	{
    		perror("Cannot send user id to the tracker\n");
    	}
        cout<<"about to create user"<<endl;

    	char nn[50]={'\0'};
        bzero(nn,'\0');
        cout<<endl;
    	read(sock,nn,50);
        cout<<endl;
    	cout<<nn<<endl;
    	
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
        if(v[0]==listrequests)
        {
            cout<<"inside listrequests if "<<endl;

            char com[listrequests.size()+1];
            strcpy(com,listrequests.c_str());
            int x;
            cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)     //command sent
            {
                perror("Cannot send command to the tracker\n");
            }

            char buf[10]={'\0'};
            cout<<endl;
            read(sock,buf,10);  //reading okay
            cout<<endl;
            status =6;
            return status;

        }
        if(v[0]==joingroup)
        {   
            cout<<"inside joingroup wala if"<<endl;

            char com[joingroup.size()+1];
            strcpy(com,joingroup.c_str());
            int x;
            cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)     //command sent
            {
                perror("Cannot send command to the tracker\n");
            }

            char buf[10]={'\0'};
            cout<<endl;
            read(sock,buf,10);  //reading okay
            cout<<endl;
            status =4;
            return status;

        }
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
            cout<<endl;
            read(sock,buf,10);  //reading okay
            cout<<endl;
            status =8;
            return status;

        }
        if(v[0]==leavegroup)
        {
            cout<<"inside leave group"<<endl;
            char com[leavegroup.size()+1];
            strcpy(com,leavegroup.c_str());
            int x;
            cout<<com<<endl;
            if ((x=send(sock,com,strlen(com),0))<0)
            {
                perror("Cannot send command to the tracker\n");
            }
            char buf[10]={'\0'};
            cout<<endl;
            read(sock,buf,10);
            cout<<endl;
            //cout<<buf<<endl;

            status=5;
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
            cout<<endl;
            read(sock,buf,10);
            cout<<endl;
            cout<<buf<<endl;

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
            cout<<endl;
            read(sock,buf,10);
            cout<<endl;
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
            cout<<endl;
            read(sock,buf,10);
            cout<<endl;
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
            cout<<endl;
            read(sock,buf,10);
            cout<<endl;
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
        if(status==4)
        {
            join_group(v,sock);
        }

        if(status==5)
        {
            leave_group(v,sock);
        }
        if(status==6)
        {
            list_requests(v,sock);
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
    cout<<endl;
    read(sock,buf,100);
    cout<<endl;
    //cout<<buf<<endl;
    if(send(sock,v[2],strlen(v[2]),0)<0)
    {
      perror("Cannot send user id to the tracker\n");
    }

    bzero(buf,100);
    cout<<endl;
    read(sock,buf,100);
    cout<<endl;
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
      cout<<endl;
      read(sock,buf,100);
      cout<<endl;
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
    cout<<endl;
    read(sock,buf, 100);
    cout<<endl;
    cout<<buf<<endl;

    char port_ad[10];
    sprintf(port_ad, "%d", port_);
    cout<<"port of client is"<<port_ad<<endl;
    send(sock,port_ad, strlen(port_ad),0);
    bzero(buf,'\0');
    cout<<endl;
    read(sock,buf,100);
    cout<<endl;
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
        cout<<endl;
        read(sock,buffer,100);
        cout<<endl;
        cout<<buffer<<endl;
        send(sock,"okay",strlen("okay"),0);
    }

}

void getcommand::leave_group(vector<char*> v, int sock)
{
    send(sock,v[1],strlen(v[1]),0);
    char comm[50]={'\0'};
    cout<<endl;
    read(sock,comm,50);
    cout<<endl;
    string com="port";
    string incoming=string(comm);

    if(com==incoming)
    {
        char port_ad[10];
        bzero(port_ad,'\0');
        sprintf(port_ad, "%d", port_);
        cout<<"port of client is"<<port_ad<<endl;
        
        send(sock, port_ad, strlen(port_ad),0);
        char buf[25]={'\0'};
        bzero(buf,'\0');
        cout<<endl;
        read(sock,buf,25);
        cout<<endl;
        cout<<buf<<endl;
    }
    else
    {
        cout<<"Wrong group id"<<endl;
    }
}

void getcommand::join_group(vector<char*> v, int sock)
{
    send(sock,v[1],strlen(v[1]),0);
    char comm[50]={'\0'};
    bzero(comm,'\0');
    cout<<endl;
    read(sock,comm,50);
    cout<<comm<<endl;
    string po=string(comm);
    string p="port";
    if(po==p)
    {
        char port_ad[10];
        bzero(port_ad,'\0');
        sprintf(port_ad, "%d", port_);
        cout<<"port of client is"<<port_ad<<endl;
        
        send(sock, port_ad, strlen(port_ad),0);
        char buf[25]={'\0'};
        bzero(buf,'\0');
        cout<<endl;
        read(sock,buf,25);
        cout<<endl;
        cout<<buf<<endl;
    }
    else
    {  
        cout<<"wrong group id"<<endl;
    }

}

void getcommand::list_requests(vector<char*> v, int sock)
{
    cout<<"inside list_requests function"<<endl;
    send(sock,v[1],strlen(v[1]),0);
    cout<<"group id sent"<<v[1]<<endl;
    char comm[50]={'\0'};
    bzero(comm,'\0');
    cout<<endl;
    read(sock,comm,50);
    cout<<"the output sent by the tracker is "<<comm<<endl;

    string out=string(comm);
    string wrong_gid="wrong gid";
    string not_owner="not an owner";
    if(out==wrong_gid)
    {
        cout<<"wrong group id"<<endl;
    }
    else
    {
        cout<<"inside else"<<endl;
        int length=stoi(out);
        vector<string> requests;
           
        char pport[10];
        bzero(pport,'\0');
        sprintf(pport,"%d",port_);

        send(sock,pport,strlen(pport),0);

        char ree[50]={'\0'};
        bzero(ree,50);
        cout<<endl;
        read(sock,ree,50);
        cout<<endl;
        string r=string(ree);
        string noo="no";
        if(r==noo)
        {
            cout<<"You are not an owner"<<endl;
        }

        else
        {
            
            for(int i=0;i<length;i++)
            {
                char re[50];
                bzero(re,50);
                cout<<endl;
                read(sock, re,50);
                cout<<endl;
                int madh=atoi(re);
                cout<<"madh "<<madh<<endl;
                string seq=string(re);
                requests.push_back(seq);
                char *kl="user received";
                send(sock,kl,strlen(kl),0);
                cout<<"user received"<<endl;
            }

            for(auto i=0;i<length;i++)
            {
                cout<<requests[i]<<endl;
            }
        
        }
    }

    cout<<"out of else"<<endl;

}