#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char* argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    /* First call to socket() function  int socket (int __domain, int __type, int __protocol) */
    /* Create a new socket of type TYPE in domain DOMAIN, using
       protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
       Returns a file descriptor for the new socket, or -1 for errors.  */
    sockfd    = socket(AF_INET, SOCK_STREAM, 0);
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    // bzero (void *__s, size_t __n)
    /* Set N bytes of S to 0.  */
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = 5003;

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //    The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        exit(1);
    }

    /* If connection is established then start communicating */
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);

    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Here is the message: %s\n", buffer);

    /* Write a response to the client */
    char newbuffer[512];
    char msg[] = "I got your message: ";
    strcpy(newbuffer, msg);
    strcpy(newbuffer + sizeof(msg) - 1, buffer);

    n = write(newsockfd, newbuffer, sizeof(newbuffer));

    if (n < 0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    if (close(newsockfd) == 0)
        printf("socket is closed\n");
    else
        perror("ERROR closing the socket");
    return 0;
}
