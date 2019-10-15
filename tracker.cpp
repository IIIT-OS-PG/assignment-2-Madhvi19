#include<iostream>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<vector>
#include<map>
#define PORT 8080
using namespace std;

struct group
{
	string gid;
	string owner;
	vector<string> members;
	map<string,string> pending_requests;
};

vector<string> sha_container;
map<string,string> users;
map<string,int> active_users;
vector<struct group> active_groups;


int mycomp(string pass, string password)
{	
	//cout<<"inside compare function"<<endl;

	if(pass.size()==password.size())
	{
		for(int i=0;i<pass.size();i++)
		{
			//cout<<"pass "<<pass[i]<<"  password "<<password[i]<<endl;
			if(pass[i]!=password[i])
			{
				return 1;
			}
		}

		return 0;
	}
	else
	{
		return 	1;
	}
}

string login_user(char b[],char buf[], int newsock)
{
	//cout<<"inside login function"<<endl;
	string user_name=string(b);
	string id_user=user_name;
	string password=string(buf);
	//cout<<"received password is "<<password<<endl;
	//cout<<"password length is "<<password.size()<<endl;

	map<string, string>::iterator itr; 

	if(users.find(user_name)!=users.end())
	{	
		itr=users.find(user_name);
		string pass=itr->second;
		//cout<<"password from map is "<<pass<<endl;
		//cout<<"lenght of password from the map user "<<pass.size()<<endl;
		int v=mycomp(pass,password);
		//cout<<"return value from mycomp is "<<v<<endl;
		if(v==0)
		{
			//cout<<"inside if "<<endl;
			string user="welcome!";
			//cout<<"user string "<<user<<endl;
			char u[user.size()+1];
			strcpy(u,user.c_str());
			cout<<u<<endl;
			if(send(newsock,u,strlen(u),0)<0)
				perror("something went wrong\n");

			char buf[10];
			bzero(buf,10);
			if(read(newsock,buf,100)<0)	
			{
				perror("Cannot read socket address of the client\n");
			}
			//cout<<"read buffer "<<buf<<endl;
			int client_port=atoi(buf);
			cout<<"client port is"<<client_port<<endl;
			active_users.insert(make_pair(user_name,client_port));


			if(send(newsock,"ok",strlen("ok"),0)<0)
			{
				perror("Something went wrong\n");
			}
			return id_user;
		}

		else
		{
			if(send(newsock,"Wrong password",strlen("Wrong password"),0)<0)
				perror("Cannot send message to the client\n");

			return "-1";
		}

	}	
	
	else
	{
		if(send(newsock,"No user exist with this user id", strlen("No user exist with this user id"),0)<0)
		{
			perror("Something went wrong\n");
		}	
		return "-1";	
	}

}

int sort_command(string v, int newsock)
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
  	if(v==createuser)
  	{
    	status=1;
    	return status;
  	}
  	if(v==login)
  	{
  		status=2;
  		return status;
  	}
  	if(v==creategroup)
  	{
    	status=3;
    	return status;
  	}
  	if(v==joingroup)
  	{
    	status=4;
    	return status;
  	}
  	if(v==leavegroup)
  	{
    	status=5;
    	return status;
  	}
  	if(v==listrequests)
  	{
    	status=6;
    	return status;
  	}
  	if(v==acceptrequest)
  	{
    	status=7;
    	return status;
  	}
  	if(v==listgroups)
  	{
    	status=8;
    	return status;
  	}
  	if(v==listfiles)
  	{
    	status=9;
    	return status;
  	}
  	if(v==download)
  	{
  		status=10;
  		return status;
  	}
  	if(v==logout)
  	{
  		status=11;
  		return status;
  	}
  	if(v==showdownloads)
  	{
    	status=12;
    	return status;
  	}
  	if(v==stop)
  	{
    	status=13;
    	return status;
  	}
  	if(v==upload)
  	{
  		return 14;
  	}
  	else
  	{
  		return 0;
  	}

}

void act(int status, int newsock)
{	
	
	if(status==1)
  	{
		cout<<"inside case create user"<<endl;
  		char b[100];
  		bzero(b,100);
  		read(newsock,b,100);
  		cout<<"b "<<b<<endl;
  		string user_name=string(b);
		send(newsock,"ok1",3,0);

		char cu[100];
		bzero(cu,100);

		read(newsock,cu, 100);
		cout<<"cu "<<cu<<endl;
		string password=string(cu);
		
		cout<<user_name<<endl;
		cout<<password<<endl;

		if(users.find(user_name)!=users.end())
		{
			if(send(newsock,"User already exists",strlen("User already exists"),0)<0)
				perror("Something went wrong\n");
			cout<<"user exists already"<<endl;
		}
		else
		{
			users.insert(make_pair(user_name,password));
			if(send(newsock,"user created",strlen("user created"),0)<0)
					perror("Something went wrong\n");
			cout<<"user created"<<endl;
		}
					
  	}
  	if(status==2)
  	{
  		//cout<<"inside act inside login"<<endl;
  		char bi[100];
  		bzero(bi,100);
  		read(newsock,bi,100);
  		//string user_name=string(b);
		send(newsock,"ok",2,0);

		char lg[100];
		bzero(lg,100);

		read(newsock,lg, 100);
		//string password=string(buf);
		//send(newsock,"ok",2,0);
		string c_id=login_user(bi,lg,newsock);
		//cout<<"current client id is "<<c_id<<endl;
	
  	}
  	if(status==3)
  	{
  		cout<<"inside create group if"<<endl;
  		char cg[100];
  		bzero(cg,100);
  		read(newsock,cg,100);
  		cout<<"Group name is "<<cg<<endl;
  		string gname=string(cg);
  		//cout<<cg<<endl;
  		//cout<<"user id of the user is "<<id_user<<endl;
  		//cout<<"client id is "<<c_id<<endl;
  		send(newsock,"ok",2,0);
  		bzero(cg,100);
  		read(newsock,cg,100);

		string cid=string(cg);
		cout<<"port of client is "<<cid<<endl;
  		group g;
  		g.gid=gname;
  		g.owner=cid;
  		g.members.push_back(cid);
  		active_groups.push_back(g);

  		vector<group>::iterator itr;
  		cout<<"beginning of for loop"<<endl;
  		for(itr=active_groups.begin();itr!=active_groups.end();++itr)
  		{
  			cout<<itr->gid<<endl;
  			cout<<itr->owner<<endl;
  			for(auto x=itr->members.begin();x!=itr->members.end();++x)
  			{
  				cout<<*x<<endl;
  			}

  		}

  		send(newsock,"group created",strlen("group created"),0);
  	
  	}

  	if(status==4)
  	{
  		cout<<"inside join group if "<<endl;
  		char jg[100]={'\0'};
  		cout<<endl;
  		read(newsock,jg,100);
  		cout<<endl;
  		//cout<<buf<<endl;
  		string gid=string(jg);

  		vector<group>::iterator itr;
  		bool flag=false;

  		for(itr=active_groups.begin();itr!=active_groups.end();++itr)
  		{
  			if(itr->gid==gid)
  			{
  				flag=true;
  				send(newsock,"port",strlen("port"),0);
  				char buffer[50]={'\0'};
  				cout<<endl;
  				read(newsock,buffer,50);
  				cout<<endl;
  				string c_port=string(buffer);
  				bool if_member=false;

  				for(auto y=itr->members.begin();y!=itr->members.end();++y)
  					{
  						if(c_port==*y)
  						{	
  							if_member=true;
  							send(newsock,"already a member",strlen("already a member"),0);
  						}
  					}

  				if(if_member==false)
  				{
  					itr->pending_requests.insert(make_pair(c_port,gid));

  					send(newsock,"request sent",strlen("request sent"),0);

               /*for(auto t=itr->pending_requests.begin();t!=itr->pending_requests.end();t++)
               {
                  cout<<t->first<<endl;
               }*/
  				}
  				 
  				break; 					
			}
  		}

  		if(flag==false)
  		{
  			send(newsock,"wrong gid",strlen("wrong gid"),0);
  		}
  	}

  	if(status==5)
  	{	
  		cout<<"inside leave_group if "<<endl;
  		char lgp[100]={'\0'};
  		cout<<endl;
  		read(newsock,lgp,100);
  		cout<<endl;
  		cout<<lgp<<endl;
  		string gid=string(lgp);
  		bool flag2=false;

  		vector<group>::iterator itr;

  		for(itr=active_groups.begin();itr!=active_groups.end();++itr)
  		{
  			if(itr->gid==gid)
  			{
  				flag2=true;
  				send(newsock,"port",strlen("port"),0);
  				char buffer[50]={'\0'};
  				cout<<endl;
  				read(newsock,buffer,50);
  				cout<<endl;
  				string c_port=string(buffer);

  				if(itr->owner==c_port)
  				{
  					send(newsock,"owner cannot leave!",strlen("owner cannot leave!"),0);  					
  				}
  				else
  				{
  					bool if_mem=false;
  					for(auto x=itr->members.begin();x!=itr->members.end();++x)
  					{
  						if(c_port==*x)
  						{	
  							if_mem=true;
  							itr->members.erase(x);
  							send(newsock,"No longer a member now",strlen("No longer a member now"),0);
						}
  					}

  					if(if_mem==false)
  					{
  						send(newsock,"You are not a member",strlen("You are not a member"),0);
  					}
  				}

  				cout<<"Present members are"<<endl;
  				for(auto xx=itr->members.begin();xx!=itr->members.end();++xx)
  				{
  					cout<<*xx<<"  ";
  				}
  				cout<<endl;

  			}

		}

		if(flag2==false)
		{
			send(newsock,"Wrong id",strlen("Wrong id"),0);
		}

  	}

   if(status==6)
   {
      cout<<"inside list request if";

      char lreq[100]={'\0'};
      cout<<endl;
      read(newsock,lreq,100);
      cout<<endl;
      cout<<"received group id is "<<lreq<<endl;
      bool flag1=false;

      string gid=string(lreq);

      vector<group>::iterator itr;

      for(itr=active_groups.begin();itr!=active_groups.end();++itr)
      {
         if(itr->gid==gid)
         {   
            cout<<"inside for loop if condition"<<endl;
            flag1=true;
            cout<<"flag1 is "<<flag1<<endl;

            int len=itr->pending_requests.size();

            char size[len+1];
            bzero(size,'\0');
            sprintf(size,"%d",len);

            send(newsock,size,strlen(size),0);

            char port_a[10]={'\0'};
            cout<<endl;
            read(newsock,port_a,10);//readin port of the client's server
            cout<<endl;
            string pp=string(port_a);
            cout<<"port of client is"<<endl;
            if(itr->owner!=pp)
            {
               cout<<"inside if not owner"<<endl;
               send(newsock,"no",strlen("no"),0);
            }
            else
            {
               send(newsock,"start",strlen("start"),0);
                    
               cout<<"inside when trying to send requests"<<endl;
               for(auto xx=itr->pending_requests.begin();xx!=itr->pending_requests.end();++xx)
               {
                  cout<<xx->first<<endl;
                  string us=xx->first;
                  char uss[us.size()+1];
                  strcpy(uss,us.c_str());
                  cout<<"port of the user is"<<uss<<endl;
                  send(newsock,uss,strlen("uss"),0);

                  char okay[10]={'\0'};
                  cout<<endl;
                  read(newsock,okay,10);
                  cout<<endl;
                  cout<<okay<<endl;
               }
            }
         }

         break;
      }

      if(itr==active_groups.end() && flag1==false)
      {
         cout<<"inside if flag1 is false"<<endl;
         send(newsock,"wrong gid",strlen("wrong gid"),0);
      }
      cout<<"leaving list_requests"<<endl;
   }

  	if(status==8)
  	{
  		cout<<"inside list groups if"<<endl;
  		char buf[10]={'\0'};
  		cout<<endl;
  		read(newsock,buf,10);
  		cout<<endl;
  		cout<<buf<<endl;
  		vector<string> gids;

  		vector<group>::iterator itr;
  		for(itr=active_groups.begin();itr!=active_groups.end();++itr)
  		{
  			//cout<<itr->gid<<endl;
  			gids.push_back(itr->gid);
		}
		int length=gids.size();
		char l[length+1]={'\0'};
		sprintf(l, "%d", length);
		send(newsock,l,strlen(l),0);
		cout<<endl;
		read(newsock,buf,10);
		cout<<endl;
		for(auto i=gids.begin();i!=gids.end();++i)
		{
			string s=*i;
			char gname[100]={'\0'};
			strcpy(gname,s.c_str());
			send(newsock,gname,strlen(gname),0);
			char buff[10]={'\0'};
			cout<<endl;
			read(newsock,buff,10);
			cout<<endl;
			cout<<buff<<endl;
		}
  	}
 
  		
	if(status==14)
	{
		char buf[10];
	  	//cout<<"inside case 14"<<endl;
	  	cout<<endl;
	  	read(newsock,buf,10);
	  	cout<<endl;
	  	int size=atoi(buf);
	  	cout<<size<<endl;
	  	send(newsock,"ok",strlen("ok"),0);
	  	while(size)
	  	{	
	  		//cout<<"inside while"<<endl;
	  		char buffer[20];
	  		bzero(buffer,20);
	  		cout<<endl;
	  		read(newsock,buffer,20);
	  		cout<<endl;
	  		cout<<buffer<<endl;
	    	string ss=string(buffer);
	    	string s=ss.substr(0,20);
	    	cout<<s<<endl;

	    	sha_container.push_back(s);
	    	bzero(buffer,20);
	    	if(send(newsock,"success",strlen("success"),0)<0)
	    		perror("Cannot send acknowledgement\n");
	    	cout<<"SHA stored"<<endl;
	    	size--;
	  	}

	  		/*cout<<endl<<"content of sha container"<<endl;
	  		for(int i=0;i<sha_container.size();i++)
	  		{
	  			cout<<sha_container[i]<<endl;
	  		}*/
	}
  	
}

void* connection_handler(void* sock)
{	

	int newsock =*(int*)sock;

	while(1)
	{
		cout<<"inside connection_handler"<<endl;
		cout<<"waiting for a command"<<endl;

		char command_buf[100];
		bzero(command_buf,100);
		int vd=0;
		cout<<endl;
		vd=read(newsock, command_buf,100);	
		cout<<endl;

		cout<<"command is "<<command_buf<<endl;
		string command=string(command_buf);
		send(newsock,"ok",2,0);
		int status=sort_command(command, newsock);
		act(status, newsock);
		
	}
}

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

	if(listen(sockfd, 4)<0)
	{
		perror("Cannot listen");
		exit(-1);
	}
	cout<<"waiting for peer"<<endl;
	int servlen=sizeof(serv);
	int newsock;
	
	pthread_t thread_ids[4];
	int no_of_clients=4;
	int x=0;
	while(no_of_clients)
	{
		if(newsock=accept(sockfd, (struct sockaddr *)&serv, (socklen_t*)&servlen))
			cout<<"connected"<<endl;
		else
			perror("Cannot connect to the client\n");

		if(pthread_create(&thread_ids[x] , NULL ,connection_handler,(void*)&newsock)<0)
        {
            perror("could not create thread");
            return 1;
        }
        no_of_clients--;
        x++;
        cout<<"newsock is"<<newsock<<endl;

        cout<<"client"<<x<<endl;
        cout<<"Handler assigned"<<endl;

	}

	
	for(int i=0;i<4;i++)
	{
		pthread_join(thread_ids[i], NULL);
	}	

	//close(newsock);

return 0;
}
