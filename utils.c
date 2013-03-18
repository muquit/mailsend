/*
**  various utility routines for mailsend
**
**
**  Development History:
**      who                  when           why
**      muquit@muquit.com    Mar-26-2001    first cut
*/

#include "mailsend.h"
#define DAY_MIN     (24 * HOUR_MIN) /* minutes in a day */
#define HOUR_MIN    60      /* minutes in an hour */
#define MIN_SEC     60      /* seconds in a minute */


/*
** returns a positive number if the file descriptor is connected to
** console. 0 if not
*/
int isInConsole(int fd)
{
    int
        rc;
    rc=_isatty(fd);
    return (rc);
}

int isInteractive(void)
{
#ifdef WINNT
        if (isInConsole(_fileno(stdout)))
            return(1);
        else
            return(0);
#else
        if (isatty(fileno(stdout)))
            return(1);
        else
            return(0);
#endif /* ! WINNT */
 
    return(0);

}
/*
** duplicate a string. exits on failure
*/
char *xStrdup (char *string)
{
    char
        *tmp;

    if (string == (char *) NULL || *string == '\0')
        return ((char *) NULL);

    /* malloc twice as much memory.  */
    tmp = (char *) malloc ((int) strlen(string)+1);

    if (tmp == (char *) NULL)
    {
        (void) fprintf(stderr,"Error: mystrdup(): memory allocation problem\n");
        exit(1);
    }
    /* it's safe to copy this way */
    (void) strcpy(tmp, string);
    return (tmp);
}

void errorMsg(char *format,...)
{
    va_list
        args;

    va_start(args,format);
    (void) fprintf (stderr,"Error: ");
    vfprintf(stderr,format,args);
    (void) fprintf(stderr,"\n");
    (void) fflush(stderr);

    if (g_log_fp != NULL)
    {
        MutilsTime
            mt;
        char
            timebuf[64];
        mutils_time_now(&mt);
        mutils_time_fmt(&mt,timebuf,sizeof(timebuf));
        (void) fprintf (g_log_fp,"%s: Error: ",timebuf);
        vfprintf(g_log_fp,format,args);
        (void) fprintf(g_log_fp,"\n");
        (void) fflush(g_log_fp);
    }
    va_end(args);
}

void showVerbose(char *format,...)
{
    va_list
        args;

    if (g_quiet) 
        return;

    if (g_verbose == 1)
    {
        if (isInConsole(_fileno(stdout)))
        {
            va_start(args,format);
            vfprintf(stdout,format,args);
            (void) fflush(stdout);
            va_end(args);
        }

        if (g_log_fp != NULL)
        {
            MutilsTime
                mt;
            char
                timebuf[64];
            mutils_time_now(&mt);
            mutils_time_fmt(&mt,timebuf,sizeof(timebuf));
            (void) fprintf(g_log_fp,"%s: " ,timebuf);
            va_start(args,format);
            vfprintf(g_log_fp,format,args);
            (void) fflush(g_log_fp);
            va_end(args);
        }

    }

}

void print_info(char *format,...)
{
    va_list
        args;
    if (g_quiet) 
        return;

    va_start(args,format);
    vfprintf(stdout,format,args);
    (void) fflush(stdout);
    va_end(args);
}

void write_log(char *format,...)
{
    va_list
        args;

    MutilsTime
        mt;
    char
        timebuf[64];

    if (g_log_fp == NULL)
        return;

    mutils_time_now(&mt);
    mutils_time_fmt(&mt,timebuf,sizeof(timebuf));

    va_start(args,format);
    (void) fprintf(g_log_fp,"%s: " ,timebuf);
    vfprintf(g_log_fp,format,args);
    (void) fflush(g_log_fp);
    va_end(args);
}


/*
** Initialize TheMail structure. returns pointer to the TheMail sturcture on
** success, NULL on failure.
*/
TheMail *newTheMail(void)
{
    TheMail
        *the_mail=(TheMail *) NULL;

    the_mail=(TheMail *) malloc(sizeof(TheMail));
    if (the_mail == (TheMail *) NULL)
    {
        (void) fprintf(stderr,"initTheMail(): malloc failed\n");
        return ((TheMail *) NULL);
    }
    memset(the_mail,0,sizeof(TheMail));

    return (the_mail);
}

Address *newAddress(void)
{
    Address
        *a;

    a=(Address *) malloc(sizeof(Address));
    if (a == (Address *) NULL)
    {
        (void) fprintf(stderr," newAddress() malloc failed\n");
    }
    memset(a,0,sizeof(Address));

    return (a);
}


int validateMusts(char *from,char *to,char *smtp_server,char *helo_domain)
{
    int
        err=0;

    if (getAddressList() == NULL)
    {
        if (to == (char *) NULL)
        {
            errorMsg("No To address/es specified. Speicfy with: -t to,to,..");
            err++;
        }
    }

    if (from == (char *) NULL)
    {
        errorMsg("No From address specified. Specify with: -f");
        err++;
    }


    if (smtp_server == (char *) NULL)
    {
        errorMsg("No SMTP server address or IP specified. Specify with -smtp");
        err++;
    }


    if (helo_domain == (char *) NULL)
    {
        errorMsg("No domain specified. Specify with: -d");
        err++;
    }

    if (err)
        return (-1);

    return (0);
}

char *askFor(char *buf,int buflen,char *label,int ask)
{

    if (label == NULL || *label == '\0')
        return (NULL);

again:
    if (isInConsole(_fileno(stdin)))
    {
        (void) fprintf(stdout,"%s",label);
        (void) fflush(stdout);
        (void) fflush(stderr);
    }

    (void) fgets(buf,buflen,stdin);
    if (*buf == '\0' || *buf == '\n')
    {
        if (ask == EMPTY_NOT_OK)
            goto again;
    }

    mutilsChopNL(buf);

    return (buf);
}

/*
** remote mailto from To
** remote mailto: from the address
**  mailto:foo => foo
** mailto: => mailto:
** mailto:x => "x"
** mailto: x => " x"
*/
char *fix_to(char *to)
{
    char
        *p=to;

    if (to == NULL || *to == '\0')
        return(p);

    /* mailto:foo */
    if (strlen(to) > 7 && mutilsStrncasecmp(to,"mailto:",7) == 0)
    {
        p=to;
        p=p+7;
        return(p);
    }

    return(p);
}

/**
 * return filepath and mime_type
 * @param str   String of the form "file,mime_type" 
 *              example: "c:\usr\local\foo.txt,text/plain"
 *              "/usr/local/foo.html,text/html"
 * @param filepath returns
 * @param fp_size  size of the buffer filepath
 * @param mime_type returns
 * @param mt_size size of the buffer mime_type
 *
 * @return 0 on success, -1 otherwise
 * 
 * Note: it will not expand "~ $" etc in the filepath
 * This function is used when a body is attached with -m flag
 *              
 */
int get_filepath_mimetype(char *str,char *filepath,int fp_size,char *mype_type,int mt_size)
{

    int
        rc=0;
    char
        *fp,
        *mt;
    if ((mt=strchr(str,ATTACHMENT_SEP)))
    {
        *mt++='\0';
    }
    else
    {
        errorMsg("Could not determine mime-type from input: %s\n",str);
        return(-1);
    }
    mutilsSafeStrcpy(mype_type,mt,mt_size);

    /* get the filepath out */
    fp=str;
    mutilsSafeStrcpy(filepath,fp,fp_size);
    return(rc);
}

void initialize_openssl(char *cipher)
{
#ifdef HAVE_OPENSSL
    static const char rnd_seed[]="my huge entropy for rng.. blah";
    SSL_CTX *ssl_ctx=(SSL_CTX *) NULL;
    SSL *ssl=NULL;
    SSL_library_init();
    SSL_load_error_strings();
    RAND_seed(rnd_seed,sizeof(rnd_seed));
    OpenSSL_add_all_algorithms();
    ssl_ctx=SSL_CTX_new(SSLv23_client_method());
    if (ssl_ctx == NULL)
    {
        errorMsg("Could not create SSL context\n");
        return;
    }
    if (cipher)
    {
        if (!SSL_CTX_set_cipher_list(ssl_ctx,cipher))
        {
            errorMsg("Could not set cipher list %s\n",cipher);
            return;
        }
    }
    ssl=SSL_new(ssl_ctx);
    if (ssl == NULL)
    {
        errorMsg("SSL_new() failed\n");
        return;
    }
    /* set ssl to msock's static */
    msock_set_ssl(ssl);

#endif /* HAVE_OPENSSL */
}

/**
 * Calculate Date for RFC822 Date: header
 *
 * @param when     time since unix epoch
 * @param datebuf  returns NULL terminated date string.
 *                  Example: Wed, 17 May 2006 13:55:35 -0400
 * @param bufsiz   size of buffer dates. It's callers responsibily to make sure 
 *                 datebuf contains enough space. It must be at least 64 bytes. 
 *                 Usually it'll contain 31 bytes.
 *
 * returns 0 on success, -1 on failure
 *
 * The code is adapted from postfix. 
 *
 * Changes I made:
 *  - I use fixed size buffers instead of dynamic ones.
 *  - I don't use %e to calcuate day of the week, instead I use %d. Windows strftime
 *    does not have %e.
 *  - I don't add timzone name.
 *
 * Reference: http://cr.yp.to/immhf/date.html
 */
int rfc822_date(time_t when,char *datebuf,int bufsiz)
{
    char
        ts1[32],
        ts2[32];

    struct tm
        *lt;

    struct tm
        gmt;

    int
        gmtoff;

    if (bufsiz < 64)
    {
        (void) fprintf(stderr,"buffer size of date must be > 31, it is %d\n",bufsiz);
        return(-1);
    }

    memset(datebuf,0,bufsiz);
    memset(ts1,0,sizeof(ts1));
    memset(ts2,0,sizeof(ts2));

    gmt=*gmtime(&when);
    lt=localtime(&when);

    gmtoff = (lt->tm_hour - gmt.tm_hour) * HOUR_MIN + lt->tm_min - gmt.tm_min;
    if (lt->tm_year < gmt.tm_year)
    gmtoff -= DAY_MIN;
    else if (lt->tm_year > gmt.tm_year)
    gmtoff += DAY_MIN;
    else if (lt->tm_yday < gmt.tm_yday)
    gmtoff -= DAY_MIN;
    else if (lt->tm_yday > gmt.tm_yday)
    gmtoff += DAY_MIN;
    if (lt->tm_sec <= gmt.tm_sec - MIN_SEC)
    gmtoff -= 1;
    else if (lt->tm_sec >= gmt.tm_sec + MIN_SEC)
    gmtoff += 1;

    /*
    ** windows strftime does not have %e, so I'll use %d instead.
    ** day 1 - 9 can be written as 01 - 09 
    */
    (void) strftime(ts1,sizeof(ts1)-1,"%a, %d %b %Y %H:%M:%S ",lt);
    if (gmtoff < -DAY_MIN || gmtoff > DAY_MIN)
    {
        (void) fprintf(stderr,"UTC time offset %d is larger than one day",gmtoff);
        return(-1);
    }

    (void) snprintf(ts2,sizeof(ts2)-1,"%+03d%02d",
            (int) (gmtoff / HOUR_MIN),(int) (abs(gmtoff) % HOUR_MIN));

    /* put everything in the buffer. it's usually 31 bytes */
    (void) snprintf(datebuf,bufsiz,"%s%s",ts1,ts2);

    /* I will not add timezone name, it's not required */

    return(0);
}

/*
** return 0 on success, -1 on failure
*/
int guess_file_type(char *file,unsigned int *flag)
{
    char
        buf[BUFSIZ];

    int
        i,
        c,
        cr_found=0,
        nbytes,
        buflen;

    FILE
        *fp=NULL;

    *flag=0;

    buflen=sizeof(buf)-1;
    fp=fopen(file,"rb");
    if (fp == NULL)
    {
        errorMsg("Could not open file %s for reading (%s)\n",
                file,ERR_STR);
        return(-1);
    }
    while(!feof(fp))
    {
        nbytes=fread(buf,1,buflen,fp);
        if ((nbytes != buflen) && ferror(fp))
        {
            errorMsg("Read error in guess_file_type\n");
            goto ExitProcessing;
        }
        for (i=0; i < nbytes; i++)
        {
            c=buf[i];
            if (cr_found)
            {
                cr_found=0;
                if (c == '\n')
                    continue;
            }
            if (c == '\r')
            {
                cr_found=1;
                *flag |= FILE_TYPE_DOS;
            }
            else if (c == '\n')
            {
                *flag |= FILE_TYPE_UNIX;
            }
            else if ((c < 32 || c > 126) && c != '\t')
            {
                *flag |= FILE_TYPE_BINARY;
                (void) fclose(fp);
                return(0);
            }
        }
    }

    if (fp)
        (void) fclose(fp);
    return(0);
ExitProcessing:
    if (fp)
        (void) fclose(fp);
    return(-1);
    
}

static int unix2dos(FILE *ifp,FILE *ofp)
{
    int
        n,
        j,
        k,
        c,
        c2;
    char
        buf2[BUFSIZ+1],
        buf[BUFSIZ+1];
    if (ifp == NULL || ofp == NULL)
        return(-1);
#ifdef WINNT
        _setmode(_fileno(ofp),_O_BINARY);
#endif /* WINNT */
        while(!feof(ifp))
        {
            n=fread(buf,1,BUFSIZ,ifp);
            (void) fprintf(stderr,"n=\"%d\"\n",n);
            k=0;
           for (j=0; j < n; j++)
            {
                c=buf[j];
                if (c == '\n')
                {
                    (void) fprintf(stderr,"j=%d\n",j);
                    buf2[k++]='\r';
                }
                buf2[k++]=c;
            }
            c2=fwrite(buf2,sizeof(char),k,ofp);
            if (c2 != k)
            {
                perror("fwrite");
                return(1);
            }
        }
}


#ifdef HAVE_OPENSSL
void print_cert_info(SSL *ssl)
{
    X509
        *cert=NULL;
    X509_NAME
        *sub=NULL,
        *issuer=NULL;
    char
        buf[BUFSIZ];

    if (!g_verbose)
        return;

    print_info("Cipher: %s\n",SSL_get_cipher(ssl));
    (void) fprintf(stdout,"Certificate information:\n");
    cert=SSL_get_peer_certificate(ssl);
    if (cert == NULL)
    {
        errorMsg("No peer certificate found\n");
        return;
    }
    /* get subject name */
    sub=X509_get_subject_name(cert);
    if (sub == NULL)
    {
        errorMsg("Could not get subject name in certificate\n");
    }
    X509_NAME_oneline(sub,buf,sizeof(buf)-1);
    (void) fprintf(stdout,"Subject: %s\n",buf);

    issuer=X509_get_issuer_name(cert);
    if (issuer == NULL)
    {
        errorMsg("Could not get issuer name in certificate\n");
    }
    X509_NAME_oneline(issuer,buf,sizeof(buf)-1);
    (void) fprintf(stdout,"Issuer: %s\n",buf);
    (void) fflush(stdout);
}

/*
** params:
**  challenge:   base64 encoded challenge
**  user         username
**  seceret      password
**
** Returns: NULL terminated base64 encoded CRAM-MD5 material on success, 
** NULL on failure
*/
char *encode_cram_md5(char *challenge,char *user,char *secret)
{
    unsigned char
        *data;

    unsigned long
        data_len;

    unsigned char
        hmac_md5[16];

    HMAC_CTX
        ctx;

    const EVP_MD
        *md5=NULL;

    unsigned int
        hmac_len;

    int
        i;

    char
        *b64;

    unsigned long
        b64len=0;

    char
        hex[33],
        buf[BUFSIZ];
    if (challenge == NULL || *challenge == '\0' ||
        user == NULL || *user == '\0' ||
        secret == NULL || *secret == '\0')
        return(NULL);

    OpenSSL_add_all_digests();

    /* decode the challenge */
    data=mutils_decode_base64((unsigned char *) challenge,strlen(challenge),&data_len);
    if (data == NULL)
    {
        errorMsg("Could not base64 decode CRAM-MD5 challenge: %s",challenge);
        return(NULL);
    }

    /* take HMAC-MD5 of the challenge*/
    md5=EVP_get_digestbyname("md5");
    HMAC_CTX_init(&ctx);
    HMAC_Init(&ctx,secret,strlen(secret),md5);
    HMAC_Update(&ctx,data,data_len);
    HMAC_Final(&ctx,hmac_md5,&hmac_len);

    /* convert the digest to hex */
    memset(hex,0,sizeof(hex));
    for (i=0; i < 16; i++)
    {
        (void) sprintf(hex+2*i,"%02x",hmac_md5[i]);
    }

    (void) snprintf(buf,sizeof(buf)-1,"%s %s",user,hex);
    /* base64 encode "user hex_digest" */
    b64=mutils_encode_base64((unsigned char *) buf,strlen(buf),&b64len);
    if (b64len <= 0)
        return(NULL);
    /* mutils_encode_base64 adds CRLF */
    if (b64len > 2)
        b64[b64len-2]='\0';
    return(b64);
}
#else

char *encode_cram_md5(char *challenge,char *user,char *secret)
{
    errorMsg("Must be compiled with OpenSSL in order to get CRAM-MD5 support\n");
    return(NULL);
}
#endif /* HAVE_OPENSSL */

