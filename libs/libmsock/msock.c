/* a quick port of my libmsock routine to MS NT */
/* muquit@Aug-20-2005  Mar 01  Eastern Standard Time 2001 */

#include "msock.h"

#ifdef HAVE_OPENSSL
static SSL      *s_ssl=NULL;
#endif /* HAVE_OPENSSL */

static SOCKET   s_sock;
static int      ssl_status=0;
static FILE     *s_logfp = NULL;

#ifdef WINNT
#undef UNICODE
#endif /* WINNT */

static int debug = 0;

void msock_set_logfp(FILE *logfp)
{
    if (logfp != NULL)
    {
        s_logfp = logfp;
    }
    else
    {
        s_logfp = stderr;
    }
}
void msock_set_debug(int d)
{
    debug = d;
}

void msock_set_socket(SOCKET sfd)
{
    s_sock=sfd;
}

SOCKET msock_get_socket(void)
{
    return(s_sock);
}

void msock_turn_ssl_on(void)
{
    ssl_status=1;
}
void msock_turn_ssl_off(void)
{
    ssl_status=0;
}
int msock_is_ssl_on(void)
{
    return(ssl_status);
}

#ifdef HAVE_OPENSSL
void msock_set_ssl(SSL *ssl)
{
    s_ssl=ssl;
}
SSL *msock_get_ssl(void)
{
    return(s_ssl);
}
#endif /* HAVE_OPENSSL */

struct in_addr *atoAddr(char *address)
{
    struct hostent
        *host;

    static struct in_addr
        saddr;

    saddr.s_addr=inet_addr(address);
    if (saddr.s_addr != -1)
        return (&saddr);

    host=gethostbyname(address);
    if (host != (struct hostent *) NULL)
        return ((struct in_addr *) *host->h_addr_list);

    return ((struct in_addr *) NULL);
}


#ifdef WINNT
void msock_print_winsock_error(void)
{
    int
        error_code = WSAGetLastError();

    LPSTR
        error_string = NULL;

    if (s_logfp == NULL)
    {
        s_logfp = stderr;
    }

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  error_code,
                  0,
                  (LPSTR) &error_string,
                  0,
                  0);
    (void) fprintf(s_logfp,"Winsock error[%d]: %s\n",
            error_code,
            error_string);
    LocalFree(error_string);
    return;
}


/* returns 0 on success -1 on failure */
int initWinSock(void)
{
    WORD
        version_requested;

    WSADATA
        wsa_data;

    int
        err;

    version_requested=MAKEWORD(2,2);
    err=WSAStartup(version_requested,&wsa_data);
    if (err != 0)
    {
        msock_print_error();
        return(-1);
    }

    return(0);
}

/*
** Adapted from libevent2
*/
int msock_get_errno(SOCKET sock_fd)
{
    int
        optval,
        optvallen = sizeof(optval);

    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK && sock_fd >= 0)
    {
        if (getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, (void*)&optval,
                       &optvallen))
            return err;
        if (optval)
            return optval;
    }
    return err;
}
#endif /* WINNT */

void msock_print_error(void)
{
    if (s_logfp == NULL)
    {
        s_logfp = stderr;
    }
#ifdef WINNT
    msock_print_winsock_error();
#else
    (void) fprintf(s_logfp,"Socket Error: [%d]: %s\n",
            errno,
            strerror(errno));
#endif /* WINNT */
}

#ifdef HAVE_GETADDRINFO
void msock_print_ipaddr(struct addrinfo *res)
{
#ifdef WINNT
    INT
        iRetval;
    struct sockaddr_in
        *sockaddr_ipv4;
    LPSOCKADDR
        sockaddr_ip;
    char
        ipstringbuffer[46];
    DWORD
        ipbufferlength = 46;
#else
    struct sockaddr
        *sa;
    char
        buf[1024];
#endif /* WINNT */

    if (res == NULL) return;
    if (debug == 0) return;

#ifdef WINNT
    /*
    ** Ref: http://msdn.microsoft.com/en-us/library/windows/desktop/ms738520%28v=vs.85%29.aspx
    */
    sockaddr_ip = (LPSOCKADDR) res->ai_addr;
    ipbufferlength = 46;
    iRetval = WSAAddressToString(sockaddr_ip, (DWORD) res->ai_addrlen, NULL, 
            ipstringbuffer, &ipbufferlength );
    if (iRetval)
    {
        (void) fprintf(stderr," WSAAddressToString failed with %u\n",
                WSAGetLastError() );
    }
    else
    {
        if (debug)
        {
            (void) fprintf(stderr," IP address: %s\n",ipstringbuffer);
        }
    }
#else
    sa = (struct sockaddr *) res->ai_addr;
    switch (sa->sa_family)
    {
        case AF_INET:
        {
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    buf,sizeof(buf) - 1);
            if (debug)
            {
                (void) fprintf(stderr," IPv4 address: %s\n",buf);
            }
            break;
        }
        case AF_INET6:
        {
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    buf,sizeof(buf) - 1);
            if (debug)
            {
                (void) fprintf(stderr," IPv6 address: %s\n",buf);
            }
            break;
        }
        default:
        {
            (void) fprintf(stderr,"Error: Uknown AF family\n");
        }
    }
#endif /* WINNT */
}
#else
void msock_print_ipaddr(void *res)
{
    ;
}
#endif /* HAVE_GETADDRINFO */


/*
** It is assumed that the socket is a blocking socket. In order to
** timeout the connect(), socket is made non-blocking and changed 
** back to blocking after a successful connect, because lot of other calls 
** are blocking. Someday I might change everything to non-blocking.
**
** returns a blocking SOCKET on success INVALID_SOCKET on failure 
*/
SOCKET clientSocket(int use, char *address,int port, int connect_timeout)
{
    SOCKET
        sock_fd;

    struct sockaddr_in
        sa;

    struct in_addr
        *addr;

    struct timeval
        tv;

    fd_set
        fdset;

    int
        eno,
        rc;
#ifdef HAVE_GETADDRINFO
    char
        service[64];

    struct addrinfo
        hints,
        *cur,
        *res,
        *ressave;
#endif /* HAVE_GETADDRINFO */


#ifdef WINNT
    rc=initWinSock();
    if (rc != 0)
        return(INVALID_SOCKET);
#endif /* WINNT */

#ifdef HAVE_GETADDRINFO
    if (debug)
    {
        (void) fprintf(stderr,"libmsock: using getaddrinfo\n");
    }
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = PF_UNSPEC;
    if (use == MSOCK_USE_IPV4)
    {
        hints.ai_family = PF_INET;
    }
    if (use == MSOCK_USE_IPV6)
    {
        hints.ai_family = PF_INET6;
    }
    (void) snprintf(service, sizeof(service) -1, "%d", port);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    rc = getaddrinfo(address, service, &hints, &res);
    if (rc != 0)
    {
        (void) fprintf(stderr,"Error: Could not find host \"%s\"\n",address);
        return(INVALID_SOCKET);
    }
    ressave = res;
    rc = (-1);
    for (cur=res; cur != NULL; cur = cur->ai_next)
    {
        if (debug)
        {
            switch (cur->ai_family)
            {
                case AF_UNSPEC:
                {
                    if (debug)
                    {
                        (void) fprintf(stderr," AF_UNSPEC\n");
                    }
                    break;
                }
                case AF_INET:
                {
                    if (debug)
                    {
                        (void) fprintf(stderr," AF_INET IPv4\n");
                    }
                    break;
                }
                case AF_INET6:
                {
                    if (debug)
                    {
                        (void) fprintf(stderr," AF_INET6\n");
                    }
                    break;
                }
            }
            msock_print_ipaddr(cur);
        }
        sock_fd = socket(cur->ai_family,
                         cur->ai_socktype,
                         cur->ai_protocol);
        if (sock_fd >= 0)
        {
            msock_make_socket_nonblocking(sock_fd);
            /*
            ** Try the current addrinfo
            ** Patch by https://code.google.com/p/mailsend/issues/detail?id=34
            */
            rc = connect(sock_fd, cur->ai_addr, cur->ai_addrlen);
            eno = msock_get_errno(sock_fd);
            if (debug)
            {
                (void) fprintf(stderr," EINPROGRESS=%d,EWOULDBLOCK=%d\n",
                        EINPROGRESS,EWOULDBLOCK);
                (void) fprintf(stderr," connect(): socket=%d,rc=%d, errno=%d\n",
                        sock_fd,rc,eno);
            }
            if (rc != 0)
            {
                if (eno == EINPROGRESS ||
                    eno == EWOULDBLOCK)
                {
                    /*
                    ** our socket is non blocking.
                    ** we need this to timout connection .
                    ** we'll make the socket blocking again at the end 
                    */
                    rc = 0;
                    if (debug)
                    {
                        (void) fprintf(stderr,"(Debug) Try socket %d\n",sock_fd);
                    }
                    break;
                }
            }
            else
            {
                msock_close_socket(sock_fd);
            }
        }
    }
    freeaddrinfo(ressave);
    if (rc != 0)
    {
        (void) fprintf(stderr,"Could not connect to %s:%d\n",address,port);
        return(INVALID_SOCKET);
    }

#else
    addr=atoAddr(address);
    if (addr == NULL)
    {
        (void) fprintf(stderr,"Error: Invalid address: %s\n",address);
        return(INVALID_SOCKET);
    }
    memset((char *) &sa,0,sizeof(sa));
    sa.sin_family=AF_INET;
    sa.sin_port=htons(port);
    sa.sin_addr.s_addr=addr->s_addr;

    /* open the socket */
    sock_fd=socket(AF_INET,SOCK_STREAM,PF_UNSPEC);
    if (sock_fd == INVALID_SOCKET)
    {
        (void) fprintf(stderr," Could not create socket\n");
        return(INVALID_SOCKET);
    }
    
    /* make the socket non-blocking */
    msock_make_socket_nonblocking(sock_fd);
    rc=connect(sock_fd,(struct sockaddr *) &sa,sizeof(sa));
#endif /* HAVE_GETADDRINFO */
    /* connect */
    if (rc != 0 )
    {
        eno = msock_get_errno(sock_fd);
        if (eno == ECONNREFUSED)
        {
            msock_print_error();
            msock_close_socket(sock_fd);
            return(INVALID_SOCKET);
        }
    }

    FD_ZERO(&fdset);
    FD_SET(sock_fd, &fdset);

    tv.tv_sec = connect_timeout;
    tv.tv_usec = 0;

    rc = select(sock_fd + 1, NULL, &fdset, NULL, &tv);
    if (rc == -1)
    {
        (void) fprintf(stderr,"Fatal select() error\n");
        msock_close_socket(sock_fd);
        return (INVALID_SOCKET);
    }
    if (rc == 0)
    {
        (void) fprintf(stderr,"Error: Connection to %s:%d timed out after %d seconds\n",
            address, port, connect_timeout);
        msock_close_socket(sock_fd);
        return (INVALID_SOCKET);
    }

    /* make the socket blocking again*/
    msock_make_socket_blocking(sock_fd);
    return(sock_fd);
}


/*
** this function writes a character string out to a socket.
** it returns -1 if the connection is closed while it is trying to
** write
*/
static int sockWrite(SOCKET sock,char *str,size_t count)
{
    size_t
        bytesSent=0;

    int
        thisWrite;

    while (bytesSent < count)
    {
       thisWrite=send(sock,str,count-bytesSent,0);
       /*
       (void) fprintf(stderr,"str=%s\n",str);
       (void) fprintf(stderr,"count=%d\n",count);
       */

       if (thisWrite <= 0)
          return (thisWrite);

        bytesSent += thisWrite;
        str += thisWrite;
    }
    return (count);
}

#ifdef HAVE_OPENSSL
int sockWriteSSL(SSL *ssl,char *str,size_t count)
{
    size_t
        bytesSent=0;

    int
        thisWrite;

    while (bytesSent < count)
    {
       thisWrite=SSL_write(ssl,str,count-bytesSent);
       if (thisWrite <= 0)
          return (thisWrite);

        bytesSent += thisWrite;
        str += thisWrite;
    }
    return (count);
}
int sockPutsSSL(SSL *ssl,char *str)
{
    return (sockWriteSSL(ssl,str,strlen(str)));
}
#endif /* HAVE_OPENSSL */

int sockPuts(SOCKET sock,char *str)
{
    return (sockWrite(sock,str,strlen(str)));
}

int sockGets(SOCKET sockfd,char *str,size_t count)
{
    int
        bytesRead;

    size_t
        totalCount=0;

    char
        buf[1],
        *currentPosition;

    char
        lastRead=0;

    currentPosition=str;

    while (lastRead != 10)
    {
        bytesRead=recv(sockfd,buf,1,0);
        if (bytesRead <= 0)
        {
            /*
            ** the other side may have closed unexpectedly
            */
            msock_print_error();
            (void) fprintf(stderr,"Error: Connection is closed unexpectedly\n");
            return (-1);
        }
        lastRead=buf[0];

        if ((totalCount < count) && (lastRead != 10)
            && (lastRead != 13))
        {
            *currentPosition=lastRead;
            currentPosition++;
            totalCount++;
        }
    }
    if (count > 0)
        *currentPosition=0;

    return (totalCount);
}
#ifdef HAVE_OPENSSL
int sockGetsSSL(SSL *ssl,char *str,size_t count)
{
    int
        bytesRead;

    size_t
        totalCount=0;

    char
        buf[1],
        *currentPosition;

    char
        lastRead=0;

    currentPosition=str;
    while (lastRead != 10)
    {
        bytesRead=SSL_read(ssl,buf,1);
        if (bytesRead <= 0)
        {
            /*
            ** the other side may have closed unexpectedly
            */
            msock_print_error();
            (void) fprintf(stderr,"Error: Connection is closed unexpectedly\n");
            return (-1);
        }
        lastRead=buf[0];

        if ((totalCount < count) && (lastRead != 10)
            && (lastRead != 13))
        {
            *currentPosition=lastRead;
            currentPosition++;
            totalCount++;
        }
    }
    if (count > 0)
        *currentPosition=0;

    return (totalCount);
}
#endif /* HAVE_OPENSSL */

/* must be called after msock_set_socket() */
/* must be called after msock_set_ssl() if SSL is on */
int msock_gets(char *str,size_t count)
{
    if (! msock_is_ssl_on())
    {
        return(sockGets(msock_get_socket(),str,count));
    }
    else
    {
#ifdef HAVE_OPENSSL
        return(sockGetsSSL(msock_get_ssl(),str,count));
#endif /* HAVE_OPENSSL */
    }
    return(-1);
}

/* must be called after msock_set_socket() */
/* must be called after msock_set_ssl() if SSL is on */
int msock_puts(char *str)
{
    if (! msock_is_ssl_on())
    {
        return(sockPuts(msock_get_socket(),str));
    }
    else
    {
#ifdef HAVE_OPENSSL
        return(sockPutsSSL(msock_get_ssl(),str));
#endif /* HAVE_OPENSSL */
    }
    return(-1);
}
void msock_close_socket(SOCKET fd)
{
#ifdef WINNT
    closesocket(fd);
#else
    close(fd);
#endif /* MSOCK_WIN32 */

}

void msock_close(void)
{
    msock_close_socket(msock_get_socket());
}

/*
** Make a socket non-blocking
*/
void msock_make_socket_nonblocking(SOCKET sock_fd)
{
#ifndef WINNT
    int
        flags;
#else
    unsigned long
        mode = 1L;
#endif /* WINNT */
    if (sock_fd == INVALID_SOCKET)
    {
        return;
    }

#ifdef WINNT
    ioctlsocket(sock_fd, FIONBIO, &mode);
#else
    /* Stevens: Page 411 */
    flags = fcntl(sock_fd, F_GETFL);
    fcntl(sock_fd, F_SETFL, (flags | O_NONBLOCK));
#endif /* WINNT */
}

/*
** Make a socket blocking
*/
void msock_make_socket_blocking(SOCKET sock_fd)
{
#ifndef WINNT
    int
        flags;
#else
    unsigned long
        mode = 0L;
#endif /* WINNT */
    if (sock_fd == INVALID_SOCKET)
    {
        return;
    }

#ifdef WINNT
    ioctlsocket(sock_fd, FIONBIO, &mode);
#else
    /* Stevens: Page 411 */
    flags = fcntl(sock_fd, F_GETFL);
    fcntl(sock_fd, F_SETFL, (flags & ~O_NONBLOCK));
#endif /* WINNT */
}
