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

#define AMS_UDP_PORT   1122
#define MAX_SOCK_SIZE  1024
char* buffer = "client:broadcast test!";
unsigned char send_buf[4] = {0xFF,0x04,0x02,0xfb};
unsigned char recvline[MAX_SOCK_SIZE + 1];

// 输入广播IP地址为255.255.255.255
// 主函数
int main(int argc, char **argv) 
{ 

	int sockfd;
        struct sockaddr_in their_addr,saUdpCli;
	struct in_addr addr;
	int broadcast = 1;
	int num = 0;
	int i = 0;
	if(argc != 2)
	{
		printf("Usage:ipaddress\n");
		exit(1);
	}
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
	{
		perror("socket function!\n");
		exit(1);
	}
	
	if((setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast))) == -1)
	{
		perror("setsockopt function!\n");
		goto EXIT;
	}
	saUdpCli.sin_family = AF_INET;
	saUdpCli.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("127.0.0.1"); // loop back ip
	saUdpCli.sin_port = htons(0);

	int err = bind(sockfd,(struct sockaddr *)&saUdpCli,sizeof(saUdpCli));
	if (err < 0)
	{
		perror("bind:");
		goto EXIT;
	}

	inet_pton(AF_INET,argv[1],&addr.s_addr);
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(AMS_UDP_PORT);
	their_addr.sin_addr = addr; 
	
	//num = sendto(sockfd,(const void*)buffer,strlen(buffer),0,(struct sockaddr*)&their_addr,sizeof(struct sockaddr));
	num = sendto(sockfd,(const void*)send_buf,sizeof(send_buf),0,(struct sockaddr*)&their_addr,sizeof(struct sockaddr));
	if(num < 0)
	{
		perror("sendto :\n");
		goto EXIT;
	}
	printf("sendto :\n");
	for(i = 0;i < num;i++)
	{
		printf("0x%02x ",send_buf[i]);
	}
	printf("\n\n");	


	
	
	while(1)
	{
		int n = recvfrom(sockfd,recvline,MAX_SOCK_SIZE,0,NULL,NULL);
		if(n <= 0)
		{
			// TBD error process
		}
		recvline[n] = 0;
		printf("recv: cnt=%d str=%s\n",n,recvline);	
		printf("recv hex :\n");
		for(i = 0;i < n;i++)
		{
			printf("0x%02x ",recvline[i]);
		}
		printf("\n\n");	
	}
EXIT:
	close(sockfd);
	return 0;
	
	
		
    	return 0;
} 

