#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
//https://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
void doprocessing(int sock);
int newsockfd;

int main(int argc, char* argv[])
{
    int sockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n, pid;

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
    char* port = getenv("NET_PORT");
    if (port)
        portno = atoi(port);
    else
        portno = 5002;

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //    The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
    serv_addr.sin_port = htons(portno);
    printf("Start server on port %d\n", portno);

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

    while (1)
    {
        /* Accept actual connection from the client */
        newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);

        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }

        /* Create child process */
        pid = fork();
        if (pid < 0)
        {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0)
        {
            /* This is the client process */
            close(sockfd);
            doprocessing(newsockfd);
            exit(0);
        }
        else
        {
            close(newsockfd);
        }
    }
    printf("Close server on port %d\n", portno);
}

void doprocessing(int sock)
{
    int readSize;
    char buffer[256];
    do
    {
        bzero(buffer, 256);
        readSize = read(sock, buffer, 255);

        if (readSize < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }

        printf("Here is the message: %s\n", buffer);
        if (strcmp("exit", buffer) == 0)
        {
            printf("Buy !!\n");
            exit(0);
        }

        if (write(sock, buffer, readSize) < 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }

    } while (1);
}
