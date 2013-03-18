/**
  @file     mutils_time.c
  @author   Muhamad A Muquit
  @date     $Date:$
  @version  $Revision:$
  @brief    Platform independ wrapper routines for time
*/

#include "mutils.h"

/* returns 0 on success, -1 on failure */
int mutils_time_now(MutilsTime *mt)
{
    int
        rc=(-1);

#ifdef WINNT
#else
    struct timeval
        tv;
#endif /* WINNT */

#ifdef WINNT
    GetSystemTimeAsFileTime(&mt->absolute);
#else
    rc=gettimeofday(&tv,NULL);
    if (rc == -1)
    {
        mutils_error("gettimeofday() failed");
        return(-1);
    }

    mt->secs=tv.tv_sec;
    mt->nsecs=tv.tv_usec * 1000;
#endif /* WINNT */

    return(0);
}

/* buf must be at least 64 bytes allocated and bufsiz must reflect that */
/* adapted from ISC time.c */
void mutils_time_fmt(MutilsTime *mt,char *buf,int bufsiz)
{
    time_t
        now;

    int
        rc;
#ifdef WINNT
    FILETIME
        localft;
    SYSTEMTIME
        st;
    char
        DateBuf[50],
        TimeBuf[50];

    static const char
        badtime[] = "99-Bad-9999 99:99:99.999";
#endif /* WINNT */

    if (bufsiz < 64)
        return;
#ifdef WINNT
    if (FileTimeToLocalFileTime(&mt->absolute, &localft) &&
        FileTimeToSystemTime(&localft, &st))
    {
        GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, "dd-MMM-yyyy",
                  DateBuf, 50);
        GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOTIMEMARKER|
                  TIME_FORCE24HOURFORMAT, &st, NULL, TimeBuf, 50);

        snprintf(buf, bufsiz, "%s %s.%03u", DateBuf, TimeBuf,
                st.wMilliseconds);
    }
    else
        snprintf(buf, bufsiz, badtime);
#else
    now=(time_t) mt->secs;
    rc=strftime(buf,bufsiz,"%d-%b-%Y %X",localtime(&now));
    if (rc != 0)
        (void) snprintf(buf+rc,bufsiz - rc,".%03u",mt->nsecs / 1000000);
    else
        (void) snprintf(buf+rc,bufsiz - 1,"99-Bad-9999 99:99:99.999"); /* from isc */
#endif /* WINNT */
}

#ifdef TEST_TIME
int main(int argc,char *argv[])
{
    char
        buf[65];

    MutilsTime
        mt;

    mutils_time_now(&mt);
    mutils_time_fmt(&mt,MUTILS_TRUE,buf,sizeof(buf));
    (void) fprintf(stderr,"%s\n",buf);
}
#endif /* TEST_TIME */
