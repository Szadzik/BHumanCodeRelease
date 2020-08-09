#include<stdio.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<sys/un.h> 
#include<string.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdlib.h>
#include<unistd.h>


#define PORT 1234
#define PUFFERSIZE 1024
#define MESSAGE_SIZE  64000

int main(int argc, char *argv[]){

struct sockaddr_in server;
struct hostent *host_info = gethostbyname("0.0.0.0");
struct in_addr ip = *(struct in_addr*)host_info->h_addr_list[0];
unsigned long addr;
char *echo_string;

int bytes_sent;
int server_sock;
int send_left;
int send_rc;
char *message_ptr;

//printf("hostinfo ist: ", puts(gethostbyname("localhost")->h_name), "%s \n");
int sock = socket(AF_INET, SOCK_STREAM, 0); 

if(sock < 0){
	perror("client is less as 0\n");
	exit(1);
}
else printf("Client created\n");

memset(&server,0,sizeof(server));
//memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
//memcpy( (char *)&server.sin_addr, host_info->h_addr, host_info->h_length);

server.sin_family = AF_INET;
server.sin_port = htons(PORT);


int connectProcess = connect(sock,(struct sockaddr*)&server, sizeof(server));
   if (connectProcess == 0) {
        printf("Client Connected \n"); 
	//while(true){}
}
    else{
        perror("Error in Connection\n");
	exit(1);
	}
echo_string = argv[0];
 //string m = malloc(argv[0]);
message_ptr = (char*)malloc (MESSAGE_SIZE);
send_left = MESSAGE_SIZE;

while (send_left > 0)
      { printf("sending \n");
       send_rc = send(sock, message_ptr, send_left, MSG_OOB);
       printf("rc ist ", send_rc, "\n");
       if (send_rc == -1)
          break;

       send_left -= send_rc;
       message_ptr += send_rc;
       }   /* End While Loop */
printf("end sendin! \n");
close(sock);
return EXIT_SUCCESS;
}
