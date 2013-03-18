#ifndef WERRNO_H
#define WERRNO_H

#ifdef WINNT

/*
** This file is auto generated on Sun Aug 26 10:12:25 -0400 2012
** Map wisock error code. Newer Visual Studio defines them
** but they do not map to Winsock error code.
*/
#undef EWOULDBLOCK
#define EWOULDBLOCK             WSAEWOULDBLOCK

#undef EINPROGRESS
#define EINPROGRESS             WSAEINPROGRESS

#undef EALREADY
#define EALREADY                WSAEALREADY

#undef ENOTSOCK
#define ENOTSOCK                WSAENOTSOCK

#undef EDESTADDRREQ
#define EDESTADDRREQ            WSAEDESTADDRREQ

#undef EMSGSIZE
#define EMSGSIZE                WSAEMSGSIZE

#undef EPROTOTYPE
#define EPROTOTYPE              WSAEPROTOTYPE

#undef ENOPROTOOPT
#define ENOPROTOOPT             WSAENOPROTOOPT

#undef EPROTONOSUPPORT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT

#undef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT

#undef EOPNOTSUPP
#define EOPNOTSUPP              WSAEOPNOTSUPP

#undef EPFNOSUPPORT
#define EPFNOSUPPORT            WSAEPFNOSUPPORT

#undef EAFNOSUPPORT
#define EAFNOSUPPORT            WSAEAFNOSUPPORT

#undef EADDRINUSE
#define EADDRINUSE              WSAEADDRINUSE

#undef EADDRNOTAVAIL
#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL

#undef ENETDOWN
#define ENETDOWN                WSAENETDOWN

#undef ENETUNREACH
#define ENETUNREACH             WSAENETUNREACH

#undef ENETRESET
#define ENETRESET               WSAENETRESET

#undef ECONNABORTED
#define ECONNABORTED            WSAECONNABORTED

#undef ECONNRESET
#define ECONNRESET              WSAECONNRESET

#undef ENOBUFS
#define ENOBUFS                 WSAENOBUFS

#undef EISCONN
#define EISCONN                 WSAEISCONN

#undef ENOTCONN
#define ENOTCONN                WSAENOTCONN

#undef ESHUTDOWN
#define ESHUTDOWN               WSAESHUTDOWN

#undef ETOOMANYREFS
#define ETOOMANYREFS            WSAETOOMANYREFS

#undef ETIMEDOUT
#define ETIMEDOUT               WSAETIMEDOUT

#undef ECONNREFUSED
#define ECONNREFUSED            WSAECONNREFUSED

#undef ELOOP
#define ELOOP                   WSAELOOP

#undef EHOSTDOWN
#define EHOSTDOWN               WSAEHOSTDOWN

#undef EHOSTUNREACH
#define EHOSTUNREACH            WSAEHOSTUNREACH

#undef EPROCLIM
#define EPROCLIM                WSAEPROCLIM

#undef EUSERS
#define EUSERS                  WSAEUSERS

#undef EDQUOT
#define EDQUOT                  WSAEDQUOT

#undef ESTALE
#define ESTALE                  WSAESTALE

#undef EREMOTE
#define EREMOTE                 WSAEREMOTE

#endif /* WINNT */

#endif /* WERRNO_H */
