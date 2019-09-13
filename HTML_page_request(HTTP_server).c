// server
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <json/json.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <time.h> 
#define PORT 8000 

int main(int argc, char const *argv[]) 

{ 
	//while(1)
	{
	    int server_fd, new_socket, valread; 
	    struct sockaddr_in address; 
	    int opt = 1; 
	    int addrlen = sizeof(address); 
	    char buffer[2048]; 
	    unsigned char *buffer_1 ;
	    int n=0;
	    char ch;
	    int len;
 		
		// HTTP Response Headers (Both can be used)

    	   // char *page = "HTTP/1.1 200 OK\nConnection: keep-alive\nAccept: application/xhtml+xml\nContent-Type: application/xhtml+xml\n";

    	    char page[8000] = "HTTP/1.1 200 OK\nConnection: keep-alive\nAccept: text/html\nContent-Type: text/html\n";
		
	  
		// Read data from File        

	   FILE *file= fopen("/home/akash/Desktop/index.html","r");   //rb for image file

	   if(NULL == file)
	   {
		printf("File pointer not valid \n");
	   }


	   fseek(file,0,SEEK_END);

	   len = ftell(file);

	   fseek(file,0,SEEK_SET);
	

	   printf("file size : %d Bytes \n",len);

	   buffer_1 = (unsigned char *) malloc(sizeof(unsigned char)*len);

	   if(NULL == buffer_1)
	   {
		printf("buffer_1 pointer not valid \n");
		return -1;
	   }


           size_t r = fread(buffer_1,1,len+1,file);

	   if(r == len)
	   {
	    printf("File read proplerly \n");
	   }

	       
	    // Creating socket file descriptor 

	    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	    { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	    } 
	       
	    // Forcefully attaching socket to the port  

	    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	    { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	    } 
	    address.sin_family = AF_INET; 
	    address.sin_addr.s_addr = INADDR_ANY; 
	    address.sin_port = htons( PORT ); 
	       
	    // Forcefully attaching socket to the port  

	    if (bind(server_fd, (struct sockaddr *)&address,  
		                         sizeof(address))<0) 
	    { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	    } 

	    if (listen(server_fd, 3) < 0) 
	    { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	    } 

	    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
		               (socklen_t*)&addrlen))<0) 
	    { 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	    }
     		//Calculate content length

       	  bzero((char *) &buffer, sizeof(buffer)); 

	  char filesize[1024],content_len[1024];
      
	  sprintf (filesize, "%d", len);

          strcat (content_len, "Content-Length:");

          strcat (content_len, filesize);

          strcat (content_len, "\n\n");


	//while(1)
	{

	   read(new_socket, buffer, 2048);

	   printf("%s",buffer );

	   bzero((char *) &buffer, sizeof(buffer));

	   send(new_socket , page , strlen(page),0); 

	   send(new_socket , content_len , strlen(content_len),0); 

	   send(new_socket , buffer_1 , strlen(buffer_1),0); 

	  memset(buffer,0,strlen(buffer));
	}
	  free(buffer_1);
	  fclose(file); 
	  close(new_socket);
	  close(server_fd);	   
	    //return 0; 
	}
}
