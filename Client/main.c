#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>

int main(int argc, char* argv[])
{
    int sockfd, portno, readSize;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    int tryAgain;

    char buffer[256];

    if (argc < 2)
    {
        fprintf(stderr, "usage %s hostname. Set port in NET_PORT env var. 5002 is default port. \n", argv[0]);
        exit(0);
    }

    char* port = getenv("NET_PORT");
    if (port)
        portno = atoi(port);
    else
        portno = 5002;

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    printf("Client started. Connect to port %d. \n Official name of host. %s Length of address. %d\n type 'exit' to "
           "quit\n",
           portno,
           server->h_name,
           server->h_length);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    /* Now ask for a message from the user, this message
      * will be read by server
   */
    tryAgain = 1;
    do
    {
        printf("Please enter the message: ");
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        tryAgain = strcmp("exit\n", buffer) != 0;

        /* Send message to the server without last char (LF/CR) */

        if (write(sockfd, buffer, strlen(buffer)-1)< 0)
        {
            perror("ERROR writing to socket");
            exit(1);
        }

        /* Now read server response */
        bzero(buffer, 256);
        readSize = read(sockfd, buffer, 255);

        if (readSize < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
        else if (readSize == 0)
        {
            perror("Zero input from server");
        }
        printf("GOT message: |%s| message  size=%d\n", buffer, readSize);

    } while (tryAgain);

    if (close(sockfd) == 0)
        printf("socket is closed\n");
    else
        perror("ERROR closing the socket");

    return 0;
}
