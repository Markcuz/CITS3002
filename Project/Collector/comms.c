#include "comms.h"
/*
 * Source predominantly done by Brian 'Beej Jorgensen" Hall, beej@beej.us from Beej's Guide to Network Programming
 * Modified by Marcus Pham
 */

int sendData(char* port, char* hostname, char* message) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    
    if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 1;
    }

    if ((numbytes = sendto(sockfd, message, strlen(message), 0,
                           p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        return 1;
    }
    
    freeaddrinfo(servinfo);
    
    printf("talker: sent %d bytes to %s\n", numbytes, hostname);
    close(sockfd);
    
    return 0;
}

int receiveData(char* port, char* receivedMessage) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("bind");
            continue;
        }
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "failed to bind socket\n");
        return 2;
    }
    
    freeaddrinfo(servinfo);
    
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, receivedMessage, MAXBUFLEN-1 , 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        return 1;
    }
    
    printf("listener: packet is %d bytes long\n", numbytes);
    receivedMessage[numbytes]='\0';
    printf("listener: packet contains \"%s\"\n", receivedMessage);
    
    //receivedMessage = &buf[0];
    
    close(sockfd);
    
    return 0;
}

