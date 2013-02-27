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



// tcp 客户端连接ok标志
int tcp_client_connect_ok_flag = 0;

int send_data_time_cnt = 0;

#define M_DELAY_TIME_MS   300   

int main(int argc, char** argv)
{
	int sockfd, recvbytes,res,flags,error,n; 
	socklen_t len;
	fd_set rset,wset;
	struct timeval tval;
	tval.tv_sec = 0;
	tval.tv_usec = 0;
	struct sockaddr_in serv_addr; 
	// 发送字符串
	char* sendData = "1234567890";
	 // 接收buffer
	char buf[1024] = "\0";

	printf("eth0-client:Version 1.003\n");
	// 创建socket描述符
	//if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
	if( sockfd == -1)
	{ 
		perror("eth0-client:socket create failed");
		return 1; 
	} 
	else
	{
		printf("eth0-client:creat sockfd = %d\n",sockfd);
	}

#if 0
	struct sockaddr_ll sll_local;
	struct ifreq ifstruct_local; 
	memset(&ifstruct_local, 0, sizeof(ifstruct_local)); 
 	strcpy(ifstruct_local.ifr_name, "eth0"); 
	if(ioctl(sockfd, SIOCGIFINDEX, &ifstruct_local)== -1)
	{
		printf("RED ioctl error\n");
                return -1;
	}

	sll_local.sll_ifindex = ifstruct_local.ifr_ifindex; 
	sll_local.sll_family = AF_PACKET; 
	sll_local.sll_protocol =  htons(ETH_P_ALL);

	if(bind(sockfd, (struct sockaddr *)&sll_local, sizeof(sll_local)) == -1) 
        { 
                printf("Error Occurred during the bind socket with NIC\n"); 
                exit(1); 
        }
	else
	{
		printf("ok to Occurred during the bind socket with NIC\n"); 
	}
#endif
	struct ifreq interface;
	strncpy(interface.ifr_ifrn.ifrn_name, "eth0", IFNAMSIZ);
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE,  (char *)&interface, sizeof(interface)) < 0) 
	{
       		perror("eth0-client:SO_BINDTODEVICE failed");
      		/* Deal with error... */
		return -1;
	}

	serv_addr.sin_family=AF_INET; 
	// 服务器tcp端口和IP地址设定
	serv_addr.sin_port=htons(5000); 
	serv_addr.sin_addr.s_addr = inet_addr("192.168.2.140"); 
	bzero(&(serv_addr.sin_zero),8);
	printf("eth0-client:connect to ip=192.168.2.140 port=5000\n");

	// 设置为非阻塞,首先获取flag,设定noblock,然后设定flag
	flags = fcntl(sockfd,F_GETFL,0);
	fcntl(sockfd,F_SETFL,flags|O_NONBLOCK);

	if ( (res = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) )< 0) 
	{ 
		// 如果返回则表示错误,直接返回		
		if(errno != EINPROGRESS)
		{
			printf("eth0-client:connect error\n");			
			return 1;
		}
	} 

	//如果server与client在同一主机上，有些环境socket设为非阻塞会返回 0 
	if(0 == res) 
	{
		tcp_client_connect_ok_flag = 1;		
		//goto done;
	} 

	while(1)
	{
		usleep( M_DELAY_TIME_MS  *1000);		
		if(tcp_client_connect_ok_flag == 0)
		{
			FD_ZERO(&rset);
			FD_SET(sockfd,&rset);
			wset = rset;
			if( ( res = select(sockfd+1, NULL, &wset, NULL,&tval) ) <= 0)
			{
				printf("eth0-client:connect time out\n");
				//close(sockfd);
				//return 1;
			}
			else
			{
				len = sizeof(error); 
				getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len); 
				if (error) 
				{ 
					fprintf(stderr, "eth0-client:Error in connection() %d - %s\n", error, strerror(error));
					//return 1;
				} 
				else
				{
					tcp_client_connect_ok_flag = 1;
					printf("eth0-client:connect ok\n");
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
					perror("eth0-client:send error!");
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
					perror("eth0-client:recv error!");
					close(sockfd);
					return 1;
				}
				buf[recvbytes] = 0x00;
				printf("eth0-client:receive num %d\n",recvbytes);
				printf("%s\n",buf);
			}
		}	
	}// while(1)	
	close(sockfd);
}

