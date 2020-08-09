#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <sys/un.h> 
#include <string.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>


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

int echo_len;

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
        
//https://stackoverflow.com/questions/18670807/sending-and-receiving-stdstring-over-socket
// https://stackoverflow.com/questions/3454007/c-sockets-can-i-only-send-characters
/**
unsigned long to_send = 123;
printf("daten; ","%lu", to_send, "\n");
unsigned long to_send_n = htons(to_send); // convert to network byte order

printf("dann: ", "%lu", to_send_n);
send(sock, (const char*)(&to_send_n), sizeof(unsigned long), 0);
*/

//echo_string = "Hello World";
//int to_send_bytes = htons(echo_string); 
//send(socket, (const char*)(&to_send_bytes), sizeof(echo_string), 0);


/**
char* dataToSend = "Hello World! This is a string of any length ...";
uint32_t dataLength = htons(strlen(dataToSend)); // Ensure network byte order 
                                                // when sending the data length
printf("uint: ", dataLength);
int a = send(sock,&dataLength ,sizeof(uint32_t) ,MSG_CONFIRM); // Send the data length
printf("a: ",a ,"\n");
int b = send(sock,dataToSend ,strlen(dataToSend),MSG_CONFIRM); // Send the string 
printf("b: ",(char)b , strlen(dataToSend), "\n");
*/
      
                                                          // data 
      echo_string = "";
message_ptr = (char*)malloc (MESSAGE_SIZE);
echo_len = strlen(echo_string);
//https://www.ibm.com/support/knowledgecenter/SSB23S_1.1.0.2020/gtpc2/cpp_send.html
while (echo_len > 0)
      { printf("sending \n");
        send_rc = send(sock, echo_string, echo_len, MSG_OOB);
       //send_rc = ioctl(sock, FIONBIO,echo_string,echo_len);
       if (send_rc == -1){
           printf("EROOR TO \n");
       break;}
          printf("rc ist ", send_rc, "\n");
       
        printf("länge davor ", echo_len, " und nachricht: ", echo_string, "\n");
       echo_len -= send_rc;
       echo_string += send_rc;
       printf("danach ", echo_len, " nachricht danach: ", echo_string, "\n");
       }  
       
printf("end sendin! \n");
close(sock);
return EXIT_SUCCESS;
}
    else{
        perror("Error in Connection\n");
	exit(1);
	}

}
