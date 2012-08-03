#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>

#define UDP_BROADCAST_SERVER_PORT   1122
#define UDP_BROADCAST_MAX_SOCK_SIZE  1024

#define UDP_BROADCAST_DEBUG 3

int udp_broadcast_server_sockfd;
struct sockaddr_in udp_broadcast_server_addr;
struct sockaddr_in udp_broadcast_client_addr;
unsigned char udp_broadcast_server_data_recv_buf[256];
unsigned char udp_broadcast_server_data_send_buf[256] = "error request";
unsigned char udp_broadcast_server_modules_info[84];
int udp_broadcast_server_init(void)
{
	long save_file_flags;
	int udp_broadcast_client_addr_size = sizeof(udp_broadcast_client_addr);
	
	udp_broadcast_server_sockfd = socket(AF_INET,SOCK_DGRAM,0);	
	if(udp_broadcast_server_sockfd == -1)
	{
		printf("Func=%s line=%d socket create error!\n",__FUNCTION__,__LINE__);
		return -1;
	}

	bzero(&udp_broadcast_server_addr,sizeof(udp_broadcast_server_addr));
	udp_broadcast_server_addr.sin_family = AF_INET;
	udp_broadcast_server_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	udp_broadcast_server_addr.sin_port = htons(UDP_BROADCAST_SERVER_PORT);

	//printf("addr:%s\n",inet_ntoa(servaddr.sin_addr));

	if(bind(udp_broadcast_server_sockfd,(struct sockaddr*)&udp_broadcast_server_addr,sizeof(udp_broadcast_server_addr)) < 0)
	{
		printf("Func=%s line=%d socket bind error!\n",__FUNCTION__,__LINE__);
		close(udp_broadcast_server_sockfd);
		return -1;
	}

	save_file_flags = fcntl(udp_broadcast_server_sockfd,F_GETFL);
	#if (UDP_BROADCAST_DEBUG >= 3)
	printf("Func=%s line=%d file flags are %ld!\n",__FUNCTION__,__LINE__,save_file_flags);
	#endif
	save_file_flags |= O_NONBLOCK;
	if(fcntl(udp_broadcast_server_sockfd,F_SETFL,save_file_flags) == -1)
	{
		#if (UDP_BROADCAST_DEBUG >= 3)		
		printf("Func=%s line=%d trying to set input socket to non-blocking!\n",__FUNCTION__,__LINE__);
		#endif
		close(udp_broadcast_server_sockfd);
		return -1;
	}
	#if (UDP_BROADCAST_DEBUG >= 3)	
	printf("Func=%s line=%d file flags are now %ld!\n",__FUNCTION__,__LINE__,save_file_flags);
	#endif

	udp_broadcast_server_fill_info();
	return 0;

}

int udp_broadcast_server_fill_info(void)
{
	int i;
	unsigned char checksum = 0;
	udp_broadcast_server_modules_info[0] = 0xfe;
	udp_broadcast_server_modules_info[1] = 84;
	udp_broadcast_server_modules_info[2] = 0x02;
	udp_broadcast_server_modules_info[3] = 0x42; // board type
	udp_broadcast_server_modules_info[4] = 0x10; // board type

	udp_broadcast_server_modules_info[5] = 192;
	udp_broadcast_server_modules_info[6] = 168;
	udp_broadcast_server_modules_info[7] = 1;
	udp_broadcast_server_modules_info[8] = 88;

	udp_broadcast_server_modules_info[9] = 0xAA;
	udp_broadcast_server_modules_info[10] = 0xBB;
	udp_broadcast_server_modules_info[11] = 0xCC;
	udp_broadcast_server_modules_info[12] = 0xDD;
	udp_broadcast_server_modules_info[13] = 0xEE;
	udp_broadcast_server_modules_info[14] = 0xFF;

	udp_broadcast_server_modules_info[15] = 1;
	udp_broadcast_server_modules_info[16] = 2;
	udp_broadcast_server_modules_info[17] = 3;
	udp_broadcast_server_modules_info[18] = 4;

	for(i = 0; i < 83;i++)
	{
		checksum += udp_broadcast_server_modules_info[i];
	}
	udp_broadcast_server_modules_info[83] = checksum;
	return 0;
	
}

int udp_broadcast_server_main(void)
{
	int recv_cnt;
	int sendto_cnt;	
	int i;
	int sin_len = sizeof(udp_broadcast_client_addr);
	recv_cnt = recvfrom(udp_broadcast_server_sockfd,udp_broadcast_server_data_recv_buf,
				sizeof(udp_broadcast_server_data_recv_buf),0,
				(struct sockaddr*)&udp_broadcast_client_addr,&sin_len);
	if((recv_cnt == -1) && (errno != EAGAIN))
	{
		printf("Func=%s line=%d errno %d\n",__FUNCTION__,__LINE__,errno);
		return -1;
	}
	else if((recv_cnt == 0) | (errno == EAGAIN))
	{
		errno = 0;
		#if (UDP_BROADCAST_DEBUG >= 3)	
		//printf("Func=%s line=%d no data yat\n",__FUNCTION__,__LINE__);
		#endif
	}
	else
	{
		errno = 0;
		#if (UDP_BROADCAST_DEBUG >= 3)	
		udp_broadcast_server_data_recv_buf[recv_cnt] = 0;
		printf("Func=%s line=%d  recv cnt=%d data:%s\n",
			__FUNCTION__,__LINE__,recv_cnt,udp_broadcast_server_data_recv_buf);
		printf("Func=%s line=%d client ip=%s port=%d\n",__FUNCTION__,__LINE__,
				inet_ntoa(udp_broadcast_client_addr.sin_addr),
				udp_broadcast_client_addr.sin_port);
		printf("recv hex :\n");
		for(i = 0;i < recv_cnt;i++)
		{
			printf("0x%02x ",udp_broadcast_server_data_recv_buf[i]);
		}
		printf("\n\n");	
		#endif
		
		if((udp_broadcast_server_data_recv_buf[0] == 0xff) && 
		   (udp_broadcast_server_data_recv_buf[1] == 0x04) &&
		   (udp_broadcast_server_data_recv_buf[2] == 0x02) &&
		   (udp_broadcast_server_data_recv_buf[3] == 0xfb))
		{
			
			sendto_cnt = sendto(udp_broadcast_server_sockfd,udp_broadcast_server_modules_info,84,
					0,(struct sockaddr *)&udp_broadcast_client_addr,sizeof(udp_broadcast_client_addr));
			#if (UDP_BROADCAST_DEBUG >= 3)				
			printf("send hex :\n");
			for(i = 0;i < sendto_cnt;i++)
			{
				printf("0x%02x ",udp_broadcast_server_modules_info[i]);
			}
			printf("\n\n");	
			#endif
		}
		else
		{
			#if 1
			sendto_cnt = sendto(udp_broadcast_server_sockfd,udp_broadcast_server_data_send_buf,
					strlen(udp_broadcast_server_data_send_buf),
					0,(struct sockaddr *)&udp_broadcast_client_addr,
					sizeof(udp_broadcast_client_addr));
			#endif
		}
	}	
	
}

int udp_broadcast_server_close(void)
{
	int close_rc;	
	close_rc = close(udp_broadcast_server_sockfd);
	if(close_rc == -1)
	{
		printf("Func=%s line=%d close error",__FUNCTION__,__LINE__);
		return -1;
	}
}


// 输入广播IP地址为255.255.255.255
// 主函数
int main(int argc, char **argv) 
{ 
	udp_broadcast_server_init();
	while(1)
	{
		usleep(10000);
		udp_broadcast_server_main();
	}
	
	
	return 0;
} 

