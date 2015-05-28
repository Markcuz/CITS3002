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
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    char buf[MAXBUFLEN];
    
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
            perror("listener: socket");
            continue;
        }
        
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }
    
    freeaddrinfo(servinfo);
    
    printf("listener: waiting to recvfrom...\n");
    
    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        return 1;
    }
    
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    
    memcpy(receivedMessage, buf, numbytes+1);
    
    printf("listener: packet contains \"%s\"\n", receivedMessage);
    
    close(sockfd);
    
    return 0;
}

int myIP(char* myIPAddr) {
    struct ifaddrs *addrs, *tmp;
    
    getifaddrs(&addrs);
    tmp = addrs;
    
    while (tmp){
        if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
            if(strcmp("eth0", tmp->ifa_name)==0) {
                strcpy(myIPAddr, inet_ntoa(pAddr->sin_addr));
                //printf("myIP: %s", myIPAddr);
            }
        }
        tmp = tmp->ifa_next;
    }
    
    freeifaddrs(addrs);
    return 0;
}
