#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
extern int errno;
#define PORT 8080
typedef struct sockaddr_in Sin;
typedef struct sockaddr S;

int clientfd;

void recv_file()
{
	for (; ; )
	{
		char buffer[66000];  int retv;   char command[100][1024];   
        size_t bufsize = 1024;
		char *str = (char *)malloc(sizeof(char)*1024);
		getline(&str,&bufsize,stdin);
        int j=0;
		if(strcmp(str,"\0")==0)
		{
			continue;
		}
        int k=0; 
		str[strlen(str)-1]='\0';

		// DIVING STRING IN SPACES 
		char* token = strtok(str, " "); 
		while (token != NULL) 
		{ 
			strcpy(command[j],token);   k++;
			token = strtok(NULL, " ");  j++; 
		}
        int flag=0;
		if(strncmp(command[0],"exit", 5) == 0)
		{
			write(clientfd,command[0],strlen(command[0]));  flag++;
			read(clientfd,str,4);
			printf("Connection with the server is closed\n");   
            flag=23;
			fflush(NULL);
			break;
		}
		else if(strncmp(command[0],"get", 4) == 0)
		{
            flag=2;
			if(flag-1 ==j)
			{
				printf("Files not provided\n");
				fflush(NULL);
			}
			else
			{
                for(int i=1;i<j;i++)
				{
                    int fd;
					fflush(NULL);   flag=0;
                    fd = open(command[i],O_CREAT|O_WRONLY|O_TRUNC, 0644);
					if(flag-1 == fd)
					{
						perror("Error in creating or opening a file");
						exit(EXIT_FAILURE);
					}
					else
					{
						write(clientfd, command[i], strlen(command[i]));    flag=65;
						printf("The name of file sent to the server is %s\n", command[i]);
						fflush(NULL);
						// CODE FOR ERROR HANDLING OF FILE
						retv=read(clientfd, buffer, 65000);     flag-=1;
                        flag+=1;
						buffer[retv] = '\0';
						fflush(NULL);
						write(clientfd, buffer, strlen(buffer));    flag-=1;
                        flag+=1;
						if(strcmp(buffer,"open")==0)
						{
							char num[20];   int bytes_per_read,n,m; 
							retv=read(clientfd,num,20);
							num[retv]='\0';
                            int length;
							length = atoi(num);
							write(clientfd,num,strlen(num));

							if(length>=65000)
							{
                                n=length/65000;
								m=length%65000;
								bytes_per_read=65000;
							}
							else
							{
								bytes_per_read=1;
								n=length/bytes_per_read;
								m=length%bytes_per_read;
							}

							//	INITIALISING POINTER
							lseek(fd, 0, SEEK_SET);

							int k=0,sum=0;
							while(n--)
							{
								retv=read(clientfd,buffer,bytes_per_read);  bytes_per_read+=1;
                                bytes_per_read-=1;
								buffer[bytes_per_read] = '\0';
								write(fd,buffer,bytes_per_read);
								sum= sum+bytes_per_read;
								long double percentage = (long double)(((long double)sum/(long double)length)*100);
								printf("\033[0;36m");   bytes_per_read--;   printf("\r%0.2Lf%% DOWNLOADING", percentage);   bytes_per_read++;   printf("\033[0m");
								fflush(NULL);
								write(clientfd,buffer,bytes_per_read);
							}
							if(m!=0)
							{
								bytes_per_read = m;
								read(clientfd,buffer,bytes_per_read);   bytes_per_read-=1;
                                bytes_per_read+=1;
								buffer[bytes_per_read] = '\0';
								write(fd,buffer,bytes_per_read);    bytes_per_read--;
								sum=length;
                                bytes_per_read+=1;
								long double percentage = (long double)(((long double)sum/(long double)length)*100);
								printf("\033[0;36m");   bytes_per_read--;   printf("\r%0.2Lf%% DOWNLOADING", percentage);   bytes_per_read++;   printf("\033[0m");
								fflush(NULL);
								write(clientfd,buffer,bytes_per_read);
							}
							read(clientfd,buffer,bytes_per_read);
							if(length == 0)
							{
                                long double per = 100;
								printf("\033[0;36m");   bytes_per_read--;   printf("\r%0.2LF%% DOWNLOADING", per);     bytes_per_read++;    printf("\033[0m");
								fflush(NULL);
							}
							printf("\nFile Download Completed\n");
                            flag--;
							fflush(NULL);
							close(fd);
						}
						else if(strcmp(buffer,"error")==0)
						{
                            flag=0;
							close(fd);
							continue;
						}
					}
				}
			}
		}
		else
		{
			printf("\033[0;31mAllowed commands are get or exit\n");     printf("\033[0m");
			fflush(NULL);
		}
	}
}

int main()
{
	// CREATING SOCKET
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd == -1)
	{
		perror("Error in socket");
		exit(0);
	}
	else
		printf("Socket is successfully created\n");

	// ASSIGNING IP, PORT
	Sin serveradd;
	int serveradd_length = sizeof(serveradd);
	bzero(&serveradd, (size_t)serveradd_length);

	serveradd.sin_family = AF_INET; 
	serveradd.sin_port = htons(PORT);

	// CONVERT ADDRESS TO BINARY
	int inet;
	inet = inet_pton(AF_INET, "127.0.0.1", &serveradd.sin_addr);
	if(inet == -1)
	{
		perror("Invalid Address");
		exit(0);
	}

	// CONNECT CLIENT TO SERVER
	int connint = connect(clientfd, (S*)&serveradd, sizeof(serveradd));
	if(connint != -1)
	{
        printf("Client is successfully Connected to Server\n");
	}
	else
    {
        perror("Error in connecting");
		exit(0);
    }
		


	// FUNCTION
	recv_file();

	// CLOSING
	close(clientfd);
	return 0;
}
