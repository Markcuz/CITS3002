#include "comms.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

SSL_CTX* init_CTX() {
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  SSL_library_init();
  OpenSSL_add_all_algorithms();
  ERR_load_crypto_strings();
  SSL_load_error_strings();
  method = SSLv3_client_method();
  ctx = SSL_CTX_new(method);
  if(ctx == NULL)
    {
      ERR_print_errors_fp(stderr);
      abort();
    }


  return ctx; 
}

void print_Certs(SSL* in_ssl)
{
  X509 *cert;
  char *buf;

  cert = SSL_get_peer_certificate(in_ssl);

  if( cert != NULL)
    {
      printf("Found cert, printing cert info:\n");
      buf = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
      printf("Subject $s\n", buf);
      free(buf);
      buf = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
      printf("Issuer: %s\n", buf);
      free(buf);
      X509_free(cert);
    }
  else
    printf("No certs! :^(\n");

}

void Get_Certificates(SSL_CTX* ctx, char* cert_file, char* key_file)
{ ///For the servers only, ie, director and bank
  if(SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM)<= 0)
    {
      ERR_print_errors_fp(stderr);
      abort();
    }

  if(SSL_CTX_use_PrivateKey_file(ctx,key_file, SSL_FILETYPE_PEM) <= 0)
    {
      ERR_print_errors_fp(stderr);
      abort();
    }

  if( !SSL_CTX_check_private_key(ctx) )
    {
      fprintf(stderr, "Private key and public certificate don't match!\n");
      abort();
    }

}


int sendData(char* port, char* hostname, char* message) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    SSL *ssl;
    SSL_CTX *ctx;

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
                             p->ai_protocol)) == -1)
        {
            perror("talker: socket");
            continue;
        }
        
        break;
    }
    
    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 1;
    }

    if ( connect(sockfd, p->ai_addr, p->ai_protocol) != 0 )
    {
	fprintf(stderr, "failed to connect\n");
	return 1;
    }

       printf("Connect returned %d \n", connect(sockfd, p->ai_addr, p->ai_protocol));
    ///SSL stuff here
    ctx = init_CTX();
    ssl = SSL_new(ctx);

    SSL_set_fd(ssl, sockfd);
 
    if(SSL_connect(ssl) != 1) //checks if connected
    {
      ERR_print_errors_fp(stderr);

    int iRet = 0;
    iRet = SSL_get_error(ssl, -1);
    switch (iRet)
    {
        case SSL_ERROR_ZERO_RETURN:
            printf("ERROR: TLS/SSL connection has been closed!\n");
            break;
        case SSL_ERROR_WANT_READ:
        case SSL_ERROR_WANT_WRITE:
            printf("ERROR: want read/write!\n");
            break;
        case SSL_ERROR_WANT_CONNECT:
        case SSL_ERROR_WANT_ACCEPT:
            printf("ERROR: socket not yet connect to the peer!\n");
            break;
        case SSL_ERROR_WANT_X509_LOOKUP:
            printf("ERROR: want x509 lookup!\n");
            break;
        case SSL_ERROR_SYSCALL:
	  printf("IT FUCKED UP ! : %s\n", strerror(errno));
	  perror("SSL_ERROR_SYSCALL");
            break;
        case SSL_ERROR_SSL:
                    printf("ERROR: failure in SSL library!n");
                    break;
        default:
            printf("No errors\n");
    }


      exit(1);
    }
    else
    {
     printf("Connected via %s encryption!\n", SSL_get_cipher(ssl));
     //CERTS
     print_Certs(ssl);
     int sw = SSL_write(ssl, message, strlen(message));
	if(sw > 0) {
	printf("Success write!\n");
	}
	else {
     printf("talker: sent %d bytes to %s\n", numbytes, hostname);
     SSL_free(ssl);
	}
    }
    
    printf("DOING STUFF \n");
    close(sockfd);
    SSL_CTX_free(ctx);
    freeaddrinfo(servinfo);
    
      
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
    SSL *ssl;
    SSL_CTX *ctx;
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
    
printf("Stuff");

    ///SSL stuff here
    ctx = init_CTX();
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);


    if(SSL_connect(ssl) == -1) //checks if connected
    {
      ERR_print_errors_fp(stderr);
	abort();
    }
    else
    {
     printf("Connected!\n");
     //CERTS
     print_Certs(ssl);
     SSL_read(ssl, buf, MAXBUFLEN-1);
     SSL_free(ssl);
     }
    ///




    freeaddrinfo(servinfo);
    
    printf("listener: waiting to recvfrom...\n");
    
/*    addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                             (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        return 1;
    }*/
    
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);
    
    //need to check this
    receivedMessage = &buf[0];
    
    close(sockfd);
    
    return 0;
}


