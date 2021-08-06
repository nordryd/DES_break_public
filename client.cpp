/**
 * TCP client based on the c client found at:
 * http://linuxhowtos.org/C_C++/socket.htm
 * @authors Jake Francis and Spencer Berg
 * @version 29 October 2017
 */
//Networking includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
//DES includes below
#include <iostream>
#include "DES_break.hpp"
//File includes below
#include <iostream>
#include <fstream>

typedef unsigned long long ullong;
using namespace std;

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  
  char buffer[256];
  if (argc < 3) 
  {
    fprintf(stderr,"usage %s hostname keyspace_port key_reporting_port\n", argv[0]);
    exit(0);
  }
    
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) 
  {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
                                                    server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");
    
  //Sending "Connected." to the server.
  char message[] = "Connected.";  
  n = write(sockfd,message,strlen(message));
  if (n < 0) 
    error("ERROR writing to socket");
    
  //Reading the minimum value from server
  bzero(buffer,256);
  n = read(sockfd,buffer,255);
  if (n < 0) 
    error("ERROR reading from socket");
  printf("Min: %s\n",buffer);
  ullong min = strtoull(buffer, NULL, 0);
    
  //Send the server a message  
  char messageTwo[] = "Min and max recieved.";
  n = write(sockfd,messageTwo,strlen(messageTwo));
  if (n < 0) 
    error("ERROR writing to socket");
    
  //This block is needed as a filler for the server
  bzero(buffer,256);
  n = read(sockfd,buffer,255);
  if (n < 0) 
    error("ERROR reading from socket");
  //--------------------------------------------------------------------

  //Reading the maximum value from server
  bzero(buffer,256);
  n = read(sockfd,buffer,255);
  if (n < 0) 
    error("ERROR reading from socket");
  printf("Max: %s\n",buffer);
  ullong max = strtoull(buffer, NULL, 0);
  
  close(sockfd);
  
  //Begin the DES with the provided PLAINTEXT and min and max.
  string PLAINTEXT = "Somebody once told me the world is gonna roll me";
  DESBreakThread dbt(5, min, max, 3500, PLAINTEXT);
  dbt.crack();
  cout << dbt.get_key() << " = " << dbt.get_key_ullong() << endl;
  
  
  //-------------After key is found second server connection------------ 
  portno = atoi(argv[3]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket(For sending key)");
  server = gethostbyname(argv[1]);
  if (server == NULL) 
  {
    fprintf(stderr,"ERROR, no such host(For sending key)\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
                                                    server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting(For sending key)");
    
  //Send the winning key-------------------------------vvvvvvvvvvvvvvvvv
  ullong key = dbt.get_key_ullong();
  sprintf(buffer, "%llu", key);
  n = write(sockfd,buffer,256);
  if (n < 0) 
    error("ERROR writing to socket");
  
  //Output the key to a file here.
  ofstream myfile;
  myfile.open ("DES_key.txt");
  myfile << "Winner, winner! Chicken dinner!\n";
  myfile << buffer;
  myfile.close();
  
  close(sockfd);
  return 0;
}
