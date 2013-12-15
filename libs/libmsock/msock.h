#ifndef MSOCK_H
#define MSOCK_H

#ifdef WINNT
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
/*#include <windows.h>*/
#include <winsock2.h>
/* http://support.microsoft.com/kb/955045 */
/* for getaddrinfo in win2k */
#include <ws2tcpip.h>
#include <wspiapi.h>
#define snprintf _snprintf
#else

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>  /* ULTRIX didn't like stat with types*/
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>  /* for inet_ntoa */
#include <time.h>  /* for ctime */
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <limits.h>


#undef SOCKET
#define SOCKET int

#undef INVALID_SOCKET
#define INVALID_SOCKET -1
#define _fileno fileno
#define _isatty isatty

#endif /* ! WINNT */

#ifdef HAVE_OPENSSL

#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#endif /* HAVE_OPENSSL */

#include "werrno.h"

#define MSOCK_USE_IPV4   0x01
#define MSOCK_USE_IPV6   0x02
#define MSOCK_USE_AUTO   0x03

/**
 * familty can be USE_IPV4, USE_IPV6 or USE_AUTO
 */
SOCKET clientSocket(int use, char *server_addr,int port, int connect_timeout);
int    sockGets(SOCKET,char *,size_t);
int    sockPuts(SOCKET sock,char *str);

void   msock_set_socket(SOCKET sock);
SOCKET msock_get_socket(void);
void   msock_turn_ssl_on(void);
void   msock_turn_ssl_off(void);
int    msock_is_ssl_on(void);
int    msock_gets(char *buf,size_t bufsiz);
int    msock_puts(char *str);
void   msock_close_socket(SOCKET fd);
void   msock_close(void);
void   msock_make_socket_nonblocking(SOCKET sock_fd);
void   msock_make_socket_blocking(SOCKET sock_fd);
void   msock_print_error(void);
void   msock_set_logfp(FILE *logfp);
void   msock_set_debug(int debug);

#ifdef WINNT
int   msock_get_errno(SOCKET sock_fd);
#else
#define msock_get_errno(sock_fd) (errno)
#endif /* WINNT */


#ifdef HAVE_OPENSSL

int    sockGetsSSL(SSL *ssl,char *buf,size_t count);
int    sockPutsSSL(SSL *ssl,char *str);
SSL    *msock_get_ssl(void);
void   msock_set_ssl(SSL *ssl);

#endif /* HAVE_OPENSSL */

#endif /* ! MSOCK_H */
