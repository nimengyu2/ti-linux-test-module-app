#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <fcntl.h> 
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h> 
#include<errno.h>
#include<netpacket/packet.h>
#include<net/if.h>
#include<net/if_arp.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<linux/if_ether.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>

int sockfd, recvbytes,res,flags,error,n; 
socklen_t len;
fd_set rset,wset;
struct timeval tval;
struct sockaddr_in serv_addr; 
// 发送字符串
char* sendData = "1234567890";
 // 接收buffer
char buf[1024] = "\0";

int slisten;	
struct sockaddr_in my_addr;
struct sockaddr_in addr;
int myport = 6000;


// tcp 客户端连接ok标志
int tcp_client_connect_ok_flag = 0;

int send_data_time_cnt = 0;

#define M_DELAY_TIME_MS   300   

int fn_init_socket(void)
{	
	tval.tv_sec = 0;
	tval.tv_usec = 0;
	// 创建socket描述符	
	if((slisten = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("ra0-server:creat socket error");
		exit(1);
	}
	else
	{
		printf("ra0-server:creat socket ok\n");
	}

	struct ifreq interface;
	strncpy(interface.ifr_ifrn.ifrn_name, "ra0", IFNAMSIZ);
	if (setsockopt(slisten, SOL_SOCKET, SO_BINDTODEVICE,  (char *)&interface, sizeof(interface)) < 0) 
	{
       		perror("ra0-server:SO_BINDTODEVICE failed");
      		/* Deal with error... */
		return -1;
	}
	else
	{
		printf("ra0-server:SO_BINDTODEVICE ok\n");
	}


	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(myport);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	int tr=1; 
 
	// kill "Address already in use" error message 
	if (setsockopt(slisten,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) 
	{ 
		perror("ra0-server:setsocslistenJ  kopt"); 
		exit(1);
	} 
	else
	{
		printf("ra0-server:setsocslistenJ  kopt ok\n");
	}

	if(bind(slisten, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
	{
        	perror("ra0-server:bind add error");
        	exit(1);
   	}
	else
	{
		printf("ra0-server:bind port=%d ok\n",myport);
	}

// 实际测试发现,不需要设定noblock就会直接跳过listen函数
#if 0
	// 设置为非阻塞,首先获取flag,设定noblock,然后设定flag
	flags = fcntl(slisten,F_GETFL,0);
	fcntl(slisten,F_SETFL,flags|O_NONBLOCK);
#endif

	if (listen(slisten, 5) == -1) {
        	perror("ra0-server:listen error");
        	exit(1);
    	}
	else
	{
		printf("ra0-server:listen ok\n");
	}
}


int main(int argc, char** argv)
{
	
	fn_init_socket();

	while(1)
	{
		usleep( M_DELAY_TIME_MS  *1000);		
		if(tcp_client_connect_ok_flag == 0)
		{
			FD_ZERO(&rset);
			FD_SET(slisten,&rset);
			wset = rset;
			if( ( res = select(slisten+1, &rset,NULL, NULL,&tval) ) <= 0)
			{
				printf("ra0-server:listen time out\n");
				//close(sockfd);
				//return 1;
			}
			else if(res > 0)
			{
				#if 0				
				len = sizeof(error); 
				getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len); 
				if (error) 
				{ 
					fprintf(stderr, "Error in connection() %d - %s\n", error, strerror(error));
					//return 1;
				} 
				else
				{
					tcp_client_connect_ok_flag = 1;
					printf("connect ok\n");
				}
				#endif
				if(FD_ISSET(slisten,&rset)) // new connection
				{
					len = sizeof(struct sockaddr);
					if((sockfd = accept(slisten,(struct sockaddr*)&addr,&len)) == -1)
					{
						perror("ra0-server:accept() error\n");
						continue;
					}
					else
					{
						printf("ra0-server:accept connection ok\n");
						printf("ra0-server:Yout got a connection from %s.\n",inet_ntoa(addr.sin_addr));
						tcp_client_connect_ok_flag = 1;
					}
				}
			} 
		}	
		else if(tcp_client_connect_ok_flag == 1)
		{
			send_data_time_cnt +=  M_DELAY_TIME_MS ;
			if(send_data_time_cnt  >= 1000)
			{
				send_data_time_cnt  = 0;
				if ( (n = send(sockfd, sendData, strlen(sendData),0) ) ==-1 ) 
				{
					perror("ra0-server:send error!");
					//close(sockfd);
					//return 1;
				}
			}
			
			FD_ZERO(&rset);
			FD_SET(sockfd,&rset);
			wset = rset;
			if( ( n = select(sockfd+1,&rset,NULL, NULL,&tval)) <= 0 )//rset没有使用过，不用重新置为sockfd
			{
				//perror("receive time out or connect error");
				//close(sockfd);
				//return -1;
			} 
			else if(n > 0)
			{
				if ((recvbytes=recv(sockfd, buf, 1024, 0)) ==-1) 
				{
					perror("ra0-server:recv error!");
					close(sockfd);
					return 1;
				}
				else if(recvbytes == 0)
				{
					printf("ra0-server:close socket\n");					
					close(sockfd);
					close(slisten);					
					fn_init_socket();
					tcp_client_connect_ok_flag  = 0;
				}
				else
				{
					buf[recvbytes] = 0x00;
					printf("ra0-server:receive num %d\n",recvbytes);
					printf("%s\n",buf);
				}
				
			}
		}	
	}// while(1)	
	close(sockfd);
}

