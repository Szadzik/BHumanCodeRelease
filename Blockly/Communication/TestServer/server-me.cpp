#include<stdio.h> 
#include <stdlib.h>  
#include<sys/types.h> 
#include<sys/socket.h> 
#include<sys/un.h> 
#include<string.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<string.h> 
#include<time.h>

#define PORT 1234
#define PUFFERSIZE 1024

static void showMessage(int client_socket){
	if(client_socket > 0)
		printf("in show %i" , client_socket, "\n");
	else printf("erro \n");
	printf("whats goping on \n");
 	char buffer[PUFFERSIZE];
	int message_size = recv(client_socket, buffer, PUFFERSIZE,0);
	if(message_size < 0){
		perror("Error at recieve");
		exit(1);
	}
	buffer[message_size] = '\0';
	printf("Message from Client is: %s ", buffer);
	
}

static void convertBack(int sock2){
	char recv_buf[sizeof(unsigned long)];
	printf("1 \n");
	recv(sock2, recv_buf, sizeof(unsigned long),0); //recieve number
	printf("2 \n");
	unsigned long recieved = ntohs(*((unsigned long*)recv_buf));
	printf("3 \n");
	printf("Message from Client is: %s ", recieved);
}

int main(){

int sock = socket(AF_INET, SOCK_STREAM,0);
int sock2;
unsigned int len;



if(sock < 0){
	perror("Server is less as 0\n");
	exit(1);
}
else
	printf("Server created\n");

struct sockaddr_in server, client;
memset(&server, 0, sizeof(struct sockaddr_in));


server.sin_family = AF_INET; // host byte order
server.sin_port = htons(PORT); // short, network byte order
server.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP

int bindProcess = bind(sock, (struct sockaddr*) &server, sizeof(server));
if(bindProcess < 0){
	perror("Error at bind");
	exit(1);
}


int listenProcess = listen(sock, 5);
if(listenProcess == -1){
	perror("Error at listen\n");
	exit(1);
}
printf("Listening...\n");

while(true){

	len = sizeof(client);
        sock2 = accept(sock, (struct sockaddr*)&client, &len);
        if (sock2 < 0)
            perror("Error at accept\n");
        printf("Bearbeite den Client mit der Adresse: %s\n",
           inet_ntoa(client.sin_addr), "\n");

        /* data from client will show up here*/
	//convertBack(sock2);
        showMessage( sock2 );
 }

}
