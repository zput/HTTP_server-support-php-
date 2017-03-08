#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <regex>


#include "XTcp.h"

#ifdef _WIN32
#include <windows.h>
#endif

//#include <iostream>

#include <thread>
using namespace std;


class httpZxc {
public:
    //unsigned int sock_connect;
    XTcp socketConnect_1;
   	void Close() {	
	
	    socketConnect_1.socketClose ();
		delete this;
	}

    
    void Main() {
    	

    	
        /*********************************recv the http client message ************************************************/ 	
		char recvBuf[256] ={'\0'};
		                             //the recv()function is block function ; so ,
		int recvLen=socketConnect_1.socketRecv(recvBuf,sizeof(recvBuf)-1);
		if(recvLen<0) {
		    printf("the data is error form the http client");
		    socketConnect_1.socketClose();
		    delete this;  //delete the function ?
		    return ;
		}
        recvBuf[recvLen]='\0';	
		printf("\n\n==========the data from the receive====================\n%s\n=================================\n",recvBuf);
		
		
			
		
/*****************************  respond the http client , just the GET message*******************************************************/		
		

			
		//GET /index.html HTTP/1.1
		//Host: 192.168.3.69
		//User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; rv:51.0) Gecko/20100101 Fi
		//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
		//Accept-Language: zh-CN,zh;q=0.8,en-US;q=0.5,en;q=0.3
		//Accept-Encoding: gzip, deflate
		//DNT: 1
		//Connection: keep-alive
		//Upgrade-Insecure-Requests: 1
			string src = recvBuf;
			string pattern = "^([A-Z]+) (.+) HTTP/1";
			regex r(pattern);
			smatch mas;
			regex_search(src,mas,r);
			if(mas.size() == 0)
			{
				printf("%s failed!\n",pattern.c_str());
				Close();
				return ;
			}
			string type = mas[1];
			string path = mas[2];
			if(type != "GET")
			{
				Close();
				return ;
			}
			
		
			
			string filename = path;
			if(path == "/")
			{
				filename = "/index.html";
			}
            
			string filepath = "www";
			filepath += filename;
			FILE *fp = fopen(filepath.c_str(),"rb");
			//	printf("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHh"); 
			if(fp == NULL)
			{
				Close();
				return;
			}
			
			//获取文件大小
			fseek(fp,0,SEEK_END);
			int filesize = ftell(fp);  //the size of  responed data;
			fseek(fp,0,0);
			printf("file size is %d\n",filesize);
			
	
		//the response message is the : 1>state line  2>message head   3> message text;
		
		std::string msg="";
		//1> 
		msg +="HTTP/1.1 200 ok ";   
		//2> [ name +':' +spaceblock +value ]
		msg +="service: XHttp\r\n" ;
		msg +="Tontent-Type: text/html\r\n";
		msg +="Content-Length: ";// the 2> is end(double \r\n) 
			
		    char bsize[128] = {0};
			sprintf(bsize,"%d",filesize);
			
		msg += bsize ;
			//rmsg += 
			//rmsg += "10\r\n";
		msg += "\r\n\r\n";
		
		//3>
        //msg +="1234567890";


			int sendSize = socketConnect_1.socketSend(msg.c_str(),msg.size());
			printf("sendsize = %d\n",sendSize);
			printf("=======send=========\n%s\n=============\n",msg.c_str());

			
			//  the respond data 
			while(1)  {
				int len = fread(recvBuf,1,sizeof(recvBuf),fp);  // restart use the recvBuf[256] data;
				if(len <=0)break;
				int re = socketConnect_1.socketSend(recvBuf,len);
				if(re<=0)break;
	
		}
		
		Close();
	}

/****************************class funcion Main end *****************************************************/

};
		
//		
//		//the response message is the : 1>state line  2>message head   3> message text;
//		
//		std::string msg="";
//		//1> 
//		msg +="HTTP/1.1 200 ok ";   
//		//2> [ name +':' +spaceblock +value ]
//		msg +="service: XHttp\r\n" ;
//		msg +="Tontent-Type: text/html\r\n";
//		msg +="Content-Length: 6\r\n\r\n";// the 2> is end(double \r\n) 
//		//3>
//		msg +="1234567890";
		
		
	//	int sendLen=socketConnect_1.socketSend(msg.c_str(),msg.size()+1);
	//	printf("\nThe http server has sent %d datas:%s\n",sendLen, msg.c_str());
		
	  //  socketConnect_1.socketClose();
	  //  delete this;
	  
	  
	  

int main(int argc, char** argv) 
{
//	#ifdef _WIN32
//	WSADATA ws;
//	WSAStartup(MAKEWORD(2,2),&ws);
//	#endif 
//	//SOCKET sock;
//	

	
		XTcp socketMonitor;
		socketMonitor.socketCreat();
		socketMonitor.port=80;
		if(argc>1) {
		  socketMonitor.port=atoi(argv[1]);  //argv[0] contained the name of main function;
		};
		 
        	printf("\n%d\n",socketMonitor.socket);
        	
        socketMonitor.socketBind();
        socketMonitor.socketListen(10);
		 
		/*************************************** accept()******************************************/ 
		while(1) {
			     
				 XTcp socketConnect;
				  
				 socketConnect=socketMonitor.socketAccept();  
				 
				 
			     //int sock_connect;   //int temp=sizeof(sockaddr); 
			                           //  socklen_t T=sizeof(sockaddr); 
				//sock_connect=accept(sock1,(sockaddr*)&cliAddr,&T);                 //(sockaddr*)&cliAddr, &temp); 
				//if(sock_connect<0 ) { break;}
				
				printf("Creat a new socket [%d]\n",socketConnect.socket); 
//				
//				char * p=inet_ntoa(cliAddr.sin_addr); 
				printf("ClientAddress ip addr:>%s   port:>%d",socketConnect.ip.c_str(), socketConnect.port ); 
				
			    httpZxc * th=new httpZxc();               //	tcpThread tThread;
				th->socketConnect_1=socketConnect;
				
				    thread sth(&httpZxc::Main ,th);   // beginning this thread;
				    sth.detach();
				
	    }

		
	   //	printf("[%d]\n",sock1);
		
     	socketMonitor.socketClose();
     	
	#ifdef _WIN32
	WSACleanup();
	#endif
	
	return 0;
}
