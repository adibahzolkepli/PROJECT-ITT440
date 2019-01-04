#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 20000
#define LENGTH 512 


void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	/* Variable Definition */
	int sockfd; 
	int nsockfd;
	char revbuf[LENGTH]; 
	struct sockaddr_in remote_addr;

	/* Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor! (errno = %d)\n",errno);
		exit(1);
	}

	/* Fill the socket address struct */
	remote_addr.sin_family = AF_INET; 
	remote_addr.sin_port = htons(PORT); 
	inet_pton(AF_INET, "127.0.0.1", &remote_addr.sin_addr); 
	bzero(&(remote_addr.sin_zero), 8);

	/* Try to connect the remote */
	if (connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "ERROR: Failed to connect to the host! (errno = %d)\n",errno);
		exit(1);
	}
	else 
		printf("[Client] Connected to server at port %d...ok!\n", PORT);

	int rows, row;
	printf("Enter the rows:");
	scanf("%d",&row);
	
	rows=htonl(row);
	write(sockfd,&rows,sizeof(rows));

	FILE *fdr,*fdw;
	char filename[25],ch,sendline[100];
	int op;
	printf("Enter file name:");
	scanf("%s",filename);

	fdr=fopen(filename,"r");
	fdw=fopen("send.txt","w+");
	ch=fgetc(fdr);
	while(ch!=EOF){
		printf("%c",ch);
		op=(int)ch;
			op=op-5;
			fprintf(fdw,"%c",op);
		ch=fgetc(fdr);	
	}
	fclose(fdw);
	fclose(fdr);
	
		char* fs_name = "send.txt";
		char sdbuf[LENGTH]; 
		printf("[Client] Sending %s to the Server... ", filename);
		FILE *fs = fopen(fs_name, "r");
		if(fs == NULL)
		{
			printf("ERROR: File %s not found.\n", filename);
			exit(1);
		}

		bzero(sdbuf, LENGTH); 
		int fs_block_sz; 
		while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
		{
		    if(send(sockfd, sdbuf, fs_block_sz, 0) < 0)
		    {
		        fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", filename, errno);
		        break;
		    }
		    bzero(sdbuf, LENGTH);
		}
		printf("Ok File %s from Client was Sent!\n", filename);
	
	close (sockfd);
	printf("[Client] Connection lost.\n");
	return (0);
}
