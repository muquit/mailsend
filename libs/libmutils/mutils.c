/* general utility routines */

#include <mutils.h>

/* file descriptor dot lock file */
static int 
    lock_fd=(-1);




void mutils_liberate_memory(void **memory)
{
    if (memory == NULL)
        return;

    if (*memory)
        free(*memory);
    *memory=(void *) NULL;
}
void mutils_liberate(void **memory)
{
    if (memory == NULL)
        return;

    if (*memory)
        free(*memory);
    *memory=(void *) NULL;
}
void mutils_free_zero(void *buf,int size)
{
    if (buf)
    {
        memset(buf,0,size);
        mutils_liberate_memory(&buf);
    }
}
 
/**
 * @brief same as mutils_acquire_memory
 */
void *mutils_acquire_memory(size_t size)
{   
    void
        *ptr;
    
    if (size == 0)
    {
        mutils_error("%s (%d) - allocation size is specified as 0\n",MUTILS_CFL);
        return(NULL);
    }
    ptr=malloc(size);
    if (ptr)
    {
        memset(ptr,0,size);
    }

    return(ptr);
}

/*
** changes the size of the memory and returns a pointer to the (possibly
** moved) block. The contents will be unchanged up to the lesser of the new
** and old sizes.
*/
void mutils_reacquire_memory(void **memory,const size_t size)
{   
    void
        *allocation;

    if (memory == NULL)
        return;

    if (*memory == (void *) NULL)
    {
        *memory=mutils_acquire_memory(size);
        return;
    }
    allocation=realloc(*memory,size);

    if (allocation == (void *) NULL)
        mutils_liberate_memory((void **) memory);

    *memory=allocation;
}


/* from my hod program, muquit, Nov-10-2000 */
static void mutils_hex_or_oct_dump(FILE *fp,int base,unsigned char *buf,int length)
{
    int
        ii,
        i;

    unsigned char
        hexbuf[16];

    int
        count=0;

    int
        line = 0;
    
    int
        c;

    unsigned char
        *p=buf;


    if (length <= 0)
        return;

    *hexbuf='\0';


    (void) fprintf (fp,
        (base == 16) ? "%11d" : "%11d",0);

    (void) fflush (fp);

    for (i=1; i<base;i++)
    {
        (void) fprintf (fp,
            (base == 16) ? "%3x" : "%4o", i);
    }

    (void) fprintf (fp,
        (base == 16) ? "   " : "    ");

    for (i=0; i <base; i++)
    {
        (void) fprintf (fp, "%x", i);
    }
    (void) fprintf (fp,"\n");



     for (ii=0; ii < length; ii++, p++)
     {
         c=*p;
         hexbuf[count] = (unsigned char) c;   
         if (count++ == 0)
         {
            if (base == 16)
            {
                /*
               (void) fprintf (fp, "%8x: ",line * 0x10);
               */
                /* print offset in decimal */
                (void) fprintf(fp,"%8d: ",line * base);
            }
            else
            {
               (void) fprintf (fp, "%8o: ",line * 0x08);
            }

         }
         (void) fprintf (fp,
            (base == 16) ? "%02x " : "%03o ", hexbuf[count-1]);

         if (count == base)
         {
            (void) fprintf (fp," ");
            for (i=0; i <base; i++)
            {
                /* 127 is non-printable - muquit@muquit.com Oct-03-2002 */
                if (hexbuf[i] >= 32 && hexbuf[i] < 127)
                   (void) fprintf (fp,"%c", hexbuf[i]);
                else
                   (void) fprintf (fp,".");
            }
            count=0;
            line++;
            (void) fprintf (fp,"\n");
         }
     }   /* while c!= EOF*/
     (void) fflush (fp);

     /*
      ** pad if necessary
      */
      if (count < base)
      {
        int
            j;

         for (j=count; j <base; j++)
         {
             (void) fprintf (fp,
                (base == 16) ? "   " : "    ");
         }

         (void) fprintf (fp," ");

         for (i=0; i < count; i++)
         {
            /* 127 is non-printable - muquit@muquit.com Oct-03-2002 */
            if (hexbuf[i] >= 32 && hexbuf[i] < 127)
            {
                (void) fprintf (fp,"%c", hexbuf[i]);
            }
            else
                (void) fprintf (fp,".");
         }
         (void) fprintf (fp,"\n");
         (void) fflush (fp);
    }   /* count < base .. */
}

void mutils_hex_dump(FILE *fp,unsigned char *buf,int length)
{
    mutils_hex_or_oct_dump(fp,16,buf,length);
}

void mutils_hex_print(FILE *fp,unsigned char *bytes,int len)
{
    int
        i;
    for (i=0; i < len; i++)
    {
        (void) fprintf(fp,"%02x ",bytes[i]);
        if ((i % 16) == 15)
            fprintf(fp, "\n");

    }
    (void) fprintf(fp,"\n");
}



void mutils_oct_dump(FILE *fp,unsigned char *buf,int length)
{
    mutils_hex_or_oct_dump(fp,8,buf,length);
}



/*
**  mutilsHowmanyCommas()
**  calculates how many commas will be added to the buffer
**
**  RCS
**      $Revision: 1 $
**      $Date: 2/24/04 8:38p $
**  Return Values:
**      no of commas 
**
**  Parameters:
**      buf     buf to scan
**
**  Side Effects:
**      none
**
**  Limitations and Comments:
**      buf must be a initialized string (at least)
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Oct-18-1997    first cut
*/
int mutilsHowmanyCommas(char *buf)
{
    int
        n=0,
        length;

    if (*buf == '\0')
        return(0);

    length=strlen(buf);

    if ((length % 3) == 0)
        n=(length/3)-1;
    else
        n=length/3;

    if ( n < 0)
        n=0;

    return(n);
}

/*
**  mutilsCommaize()
**  add a comma after every 3rd digit from right
**
**  RCS
**      $Revision: 1 $
**      $Date: 2/24/04 8:38p $
**  Return Values:
**      none
**
**  Parameters:
**      buf     buf to modify
**
**  Side Effects:
**      buf is modified
**
**  Limitations and Comments:
**      buf must have enough space to hold the extra , characters
**
**  Development History:
**      who                  when       why
**      ma_muquit@fccc.edu   no idea    first cut
**                           Oct-18-97  added dynamic buffer 
**                           May-26-1999 rewrote as the old version has
**                           memory overstepping bug. Code adapted from
**                           C-Snippets, file commaflt.c by Bruce Wedding and
**                           and Kurt Kuzba
*/
void mutilsCommaize(char *buf)
{
    char
        *pbuf=(char *) NULL;

    int
        bf=0,
        cm=0,
        tm=0;

    if (*buf != '\0')
    {
        pbuf=mutilsStrdup(buf);
        if (pbuf == (char *) NULL)
            return; /* malloc failed, return quitely */

        mutilsReverseString(pbuf);
        while ((buf[bf++]=pbuf[tm++]) != 0)
        {
            if(++cm % 3 == 0 && pbuf[tm])
                buf[bf++]=',';
        }
        if (pbuf)
            (void) free((char *) pbuf);

        mutilsReverseString(buf);
    }
}


/*
** NULL terminate the buffer at the first sight of a non-digit character
*/

/*
**  mutilsCleanBuf()
*   NULL terminate the buffer at the first sight of a non-digit character
**
**  Parameters:
**  char    *buf            the buffer to clean
**  char    *bytes_in_buf   the buffer size
**  int     *length         returns, bytes in buffer after cleaning
**
**  Return Values:
**
**
**  Limitations and Comments:
**  buf is modified
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
void mutilsCleanBuf(buf,bytes_in_buf,length)
char
    *buf;
int
    bytes_in_buf;
int
    *length;
{
    int
        i;


    int
        c=0;
    for (i=0; i < bytes_in_buf; i++)
    {
        if (!isdigit(buf[i]))
        {
            buf[i]='\0';
            break;
        }
        c++;
    }

    *length=c;
}

/*
**  mutilsParseURL()
**  parse a URL of the form
**          http://host:port/thepage.html
**          http://host/thepage.html
**
**  Parameters:
**  char *url               the url to parse
**  char *hostname          the hostname (returns)
**  int  hostname_len       allocated space length in hostname
**  int  *port              the port (returns)
**  char *page              the page (returns)
**  int page_len            the alloacated space length in page
**
**  Return Values:
**  0 on success
**  -1 on failure
**
**  Limitations and Comments:
**  - not much error checking. 
**  - hostname and page must have enough space preallocated by the caller.
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-10-1999    first cut
**                           Jul-20-1999    handle buffer overflow
*/
int mutilsParseURL(url,hostname,hostname_len,port,page,page_len)
char
    *url;

char
    *hostname;

int
    hostname_len;

int
    *port;

char
    *page;

int
    page_len;

{
    char
        tmpbuf[32],
        *q,
        *r,
        *p,
        *ptmp;

    int
        tmpbuf_len,
        hlen=0,
        tlen=0;

    if (url == NULL || *url == '\0')
        return(-1);

    /* initialize */
    *port=80;
    *hostname = *page = *tmpbuf = '\0';

    tmpbuf_len=sizeof(tmpbuf);

    ptmp=mutilsStrdup(url);
    if (!ptmp)
        return (-1);
    if (strlen(ptmp) < 7)
    {
        (void) free(ptmp);
        return (-1);
    }

    /* skip http:// */
    if ( (ptmp[0] == 'h' || ptmp[0] == 'H') &&
         (ptmp[1] == 't' || ptmp[1] == 'T') &&
         (ptmp[2] == 't' || ptmp[2] == 'T') &&
         (ptmp[3] == 'p' || ptmp[3] == 'P') &&
         (ptmp[4] == ':') &&
         (ptmp[5] == '/') &&
         (ptmp[6] == '/'))
    {
        p = ptmp+7;
    }
    else
    {
        (void) free(ptmp);
        return (-1);
    }

    hlen=0;
    tlen=0;
    q=hostname;
    r=tmpbuf;
    for (; (*p != '/') && (*p != '\0'); p++)
    {
        /* get host part out */
        if (*p != ':' && !isdigit(*p))
        {
            if (++hlen > hostname_len-1)
            {
                continue;
            }
            *q++ = *p;
        }

        /* get port out if any */
        if (isdigit(*p))
        {
            if (++tlen > tmpbuf_len-1)
                continue;
            *r++ = *p;
        }
    }
    /* NULL terminate */
    *q='\0';
    *r='\0';


    if (*tmpbuf != '\0')
        *port=atoi(tmpbuf);

    /* the rest is the page */
    if (*p == '\0')
        return (-1);

    (void) mutilsStrncpy(page,p,page_len);

    (void) free(ptmp);
    return (0);
}



/*
**  mutilsSpacesToChar()
**      converts all spaces to a single character
**
**  Parameters:
**      char    *str    - spaces to collapse from
**      int     c       - to this character
**
**  Return Values:
**      pointer to the string
**
**  Limitations and Comments:
**      str is modified.
**
** adapted from C snipptes lib: lv1ws 
**
**  Development History:
**      who                  when           why
**      muquit@lucent.com    Mar-27-2001    first cut
*/
char *mutilsSpacesToChar(char *str,int c)
{
    char
        *ibuf,
        *obuf;

    register int
        i,
        n;

    if (str)
    {
        ibuf=obuf=str;
        i=n=0;

        while (*ibuf)
        {
            if (isspace(*ibuf) && n)
                ibuf++;
            else
            {
                if (!isspace(*ibuf))
                    n=0;
                else
                {
                    *ibuf=c;
                    n=1;
                }
                obuf[i++] = *ibuf++;
            }
        }
        obuf[i]='\0';

    }
    return (str);
}

/*
**  mutilsRmallws()
**  remove all whitespace (leading or trailing) from a string.
**
**  Parameters:
**  char    *str
**
**  Return Values:
**  pointer to the string
**
**  Limitations and Comments:
**  str is modified. borrowed from public domain c snippets libraray.
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
char *mutilsRmallws(char *str)
{
    char
        *obuf,
        *nbuf;

    if (str)
    {
        for (obuf=str, nbuf=str; *obuf; ++obuf)
        {
            if (!isspace(*obuf))
                *nbuf++ = *obuf;
        }
        *nbuf='\0';
    }
    return (str);
}
/*
**  mutilsStristr()
**  case insensitive version of ANSI strstr()
**
**  Parameters:
**  char    *s  hay stack
**  char    *t  needle
**
**  Return Values:
**  pointer to hay stack  where the needle is found
**  NULL if not found
**
**  Limitations and Comments:
**  from swish package by kevin h , kevin called it lstrstr()
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
char *mutilsStristr(s,t)
char
    *s;
char
    *t;
{

int
    i,
    j,
    k,
    l;

    for (i = 0; s[i]; i++)
    {
        for (j = 0, l = k = i; s[k] && t[j] &&
            tolower(s[k]) == tolower(t[j]); j++, k++)
            ;
        if (t[j] == '\0')
            return s + l;
    }
    return NULL;
}

/*
**  mutilsIsinname()
**  checks if the mask fits in the string
**
**  RCS
**      $Revision: 1 $
**      $Date: 2/24/04 8:38p $
**  Return Values:
**      1       if fits
**      0       if not
**      -1      if there's a memory allocation problem
**
**  Parameters:
**      string      source string
**      mask        mask string
**
**  Example:
**      www.fccc.edu  *.fccc.edu        fits
**      www.fccc.edu  www*              fits
**      www.fccc.edu  *fccc*            fits
**      132.138.4.6   132*              fits
**      etc.....
**
**  Side Effects:
**      none
**
**  Limitations and Comments:
**      borrowed from swish by Kevin H
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Oct-18-1997    first cut
*/
int mutilsIsinname(string,mask)
char
    *string;
char
    *mask;
{
    int
        i,
        j;

    char
        firstchar,
        lastchar,
        *tempmask;

    if ((*string == '\0') || (*mask == '\0'))
        return (0); /* mm */

    if (!strcmp(mask, "*"))
            return 1;

    firstchar=mask[0];
    lastchar=mask[(strlen(mask) - 1)];
    tempmask=(char *) malloc(strlen(mask)*sizeof(char)+1);
    if (tempmask == NULL)
    {
        (void) fprintf(stderr,"libmutils.a-mutilsIsinname(), malloc failed\n");
        return(-1);
    }

    for (i = j = 0; mask[i]; i++)
        if (mask[i] != '*')
            tempmask[j++] = mask[i];

    tempmask[j]='\0';
    if (firstchar == '*')
    {
        if (lastchar == '*')
        {
            if ((char *) mutilsStristr(string, tempmask))
            {
                free(tempmask);
                return 1;
            }
        }
        else
        {
            if ((char *) mutilsStristr(string, tempmask) ==
                 string + strlen(string) - strlen(tempmask))
            {
                free(tempmask);
                return 1;
            }
        }
    }
    else if (lastchar == '*')
    {
        if ((char *) mutilsStristr(string, tempmask) == string)
        {
            free(tempmask);
            return 1;
        }
    }
    else
    {
        /*
        ** changed from strcmp(), patch sent my
        ** Takeshi OKURA <okura@osa.ncs.co.jp>
        ** Oct-30-1997
        */
        if (!mutilsStrcasecmp(string, tempmask))
        {
            free(tempmask);
            return 1;
        }
    }
    free(tempmask);

    return 0;
}
/*
**  mutilsGetTIme()
**  get current time, ex: Wed Jun 30 21:49:08 1993
**
**  Parameters:
**  none
**
**  Return Values:
**  pointer a string containing time
**
**  Limitations and Comments:
**  remove the trailing new line from ctime(). subsequent call will
**  destroy the buffer. ctime() retuns pointer to a static buffer.
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
char *mutilsGetTime ()
{
    time_t
        tm;

    char
        *times;

    tm=time (NULL);
    times=ctime(&tm);
    times[(int) strlen(times)-1] = '\0';
    return (times);
}   

/* chop new line from a string */
/* string is modifiled */
char mutilsChopNL(string)
char
    *string;
{
    char
        c;
    char
        *ptr;

    c='\0';

    if (string == NULL || *string == '\0')
        return(c);

    for (ptr = string; *ptr; ptr++);

    if (ptr != string)
    {
        c = *(--ptr);

        if (c == '\n')
        {
            *ptr='\0';
        }
    }

    return (c);
}

/*
 *  mutilsStripLeadingSpace () -   strips leading space/s from a string
 *
 *  Description:
 *      This function strips leading spaces from a string and
 *      null terminates it.
 *
 *  Input Parameters:
 *      char    *str
 *
 *  Output Parameters:
 *      char    *str
 *
 *  Return Values:
 *      one
 *
 *  Side Effects:
 *      str is modified
 *
 *  Limitations and Comments:
 *      str must points to a pre-allocated static or dynamic space
 *
 *  Development History:
 *      who                  when       why
 *      MA_Muquit@fccc.edu   18-Jun-96  first cut
 */
void mutilsStripLeadingSpace(char *s)
{
    int i,
        n=0;

    char
        *ls;

    if (s == NULL || *s == '\0')
        return;

    for (i=0; i < (int) strlen(s); i++)
    {
        if ((s[i] == ' ' ) || (s[i] == '\t'))
            n++;
        else
            break;
    }

    if (n)
    {
        ls= (char *) malloc(strlen(s)*sizeof(char)+1);
        if (ls != (char *) NULL)
        {
           (void) strcpy (ls,s+n);
           (void) strcpy (s,ls);
           (void) free ((char *) ls);
        }
    }
}

/*
**  mutilsStripTrailingSpace() 
**  strips trailing spaces at the end of a string.
**
**  Parameters:
**  char *str   the string
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  the string is modified
**
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-04-1998    first cut
*/
void mutilsStripTrailingSpace (char *str)
{
    register int
        i;

    if (str == NULL || *str == '\0')
        return;
    for ((i=(int) strlen(str)-1); i >= 0; i--)
    {
        if ((str[i] == ' ') || (str[i] == '\t'))
        {
            str[i] = '\0';
        }
        else
            break;
    }
}

/*
** make a temporary filename 
** filename - returns
*/
#if 0
int mutilsTmpFilename(char *filename)
{
#define TMP_DIR      "/tmp"
#define TMP_TEMPLATE "%s/editdb.XXXXXX"    
    char
        *dir;

    dir=(char *) getenv("TMPDIR");
    if (dir == (char *) NULL)
        dir=TMP_DIR;

    (void) sprintf(filename,TMP_TEMPLATE,dir);
#ifdef HAVE_MKSTEMP
    return(mkstemp(filename));
#else
    return(mktemp(filename));
#endif /* ! HAVE_MKSTEMP */
}
#endif /* 0 */


/*
 *  basename()  -   return a file's basename
 *
 *  RCS:
 *      $Revision: 1 $
 *      $Date: 2/24/04 8:38p $
 *
 *  Security:
 *      Unclassified
 *
 *  Description:
 *      Basename parses a char string containing a path name and
 *      returns a pointer to the file's base name.
 *
 *  Input Parameters:
 *      char *path  -   the fully qualified path to parse
 *
 *  Output Parameters:
 *      None.
 *
 *  Return Values:
 *      (char *)    -   a pointer to the start of the base name within
 *                      'path'
 *
 *  Side Effects:
 *      None.
 *
 *  Limitations and Comments:
 *      Basename handles both DOS and UNIX path separators; it also
 *      handles the DOS device delimiter.
 *      
 *      I renamed it to mutilsBasename() as basename() will have collision
 *      now a days. -- mm, augh-11-1999
 *
 *      Note that this function can return NULL.
 *
 *  Development History:
 *      3/26/92, jps, first cut
 */
char *mutilsBasename(path)
char
    *path;
{
    char
        *cptr;

    for (cptr = path + strlen(path); cptr >= path; --cptr)
    {
        switch (*cptr)
        {
            case ':':
            case '/':
            case '\\':
                return ++cptr;
        }
    }
    return (path);
}


/*
**  mutilsDotLock()
**  open a file for locking purpose. If the system is Unix, use
**  Kernel lock on the file. 
**
**  Parameters:
**  char *lockfile_path  - full path of the lock file
**  char *errbuf         - feels this errbuf in case of error
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  if you're on Unix, make sure the partition is not NFS mounted, if i
**  is NFS mounted, the counter might hang.
**  errbuf must have enough space to hold the message
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-16-1999    first cut
*/

static char
    s_lockfile[BUFSIZ];

void mutilsDotLock(char *filepath,char *errbuf)
{
    *errbuf='\0';
    *s_lockfile='\0';

#ifdef SYS_WIN32
        lock_fd=sopen(filepath,_O_RDWR|_O_CREAT,SH_DENYWR,_S_IREAD|_S_IWRITE);
#else
        lock_fd=open(filepath,O_RDWR|O_CREAT,0644);
#endif

    if (lock_fd < 0) /* open failed */
    {
        (void) sprintf(errbuf,"Could open counter database lock file for writing:\n%s",filepath);
        return;
    }
    (void) strcpy(s_lockfile,filepath);

#ifdef SYS_UNIX
        mutilsSetLock(lock_fd);
#endif

}


/* unlock the file */
void mutilsDotUnlock(int delete)
{
    if (lock_fd >= 0)
    {
        /* closing also unlocks kernel locking */
        (void) close(lock_fd);
        lock_fd=(-1);
        if (delete == 1)
            unlink(s_lockfile);
    }
}


#ifdef SYS_UNIX
void mutilsSetLock (int fd)
{
#ifdef HAVE_FLOCK
    (void) flock(fd,LOCK_EX);
#else
    lseek(fd,0L,0);
    (void) lockf(fd,F_LOCK,0L);
#endif
}
    
void mutilsUnsetLock (int fd)
{
#ifdef HAVE_FLOCK 
    (void) flock(fd,LOCK_UN);
#else
    lseek(fd,0L,0);
    (void) lockf(fd,F_ULOCK,0L);
#endif
}   
#endif  /* SYS_UNIX */


/*
** mutilsWhich()
** returns 0 if the program is found in path
**         -1 otherwise
**
** modified from: /usr3/doc/which-1.0/which.c on RH 5.2
*/

int mutilsWhich(char *name)
{
#ifndef X_OK
#define X_OK    0x01
#endif /* X_OK */

    char
        s,
        *p,
        *path,
        szbuf[BUFSIZ];
    int
        found=0,
        len;

    /* check if the program exists and executable */
    if (access(name,X_OK) == 0)   /* found it */
        return(0);

    /* go through the PATH */
    path=getenv("PATH");
    if (path == NULL)
        return(-1);

    p=path;
    found=0;
    while (*p != '\0' && found == 0)
    {
        len=0;
        while (*p != ':' && *p != '\0')
        {
            len++;
            p++;
        }
        s=(char) *p;
        *p='\0';
        (void) sprintf(szbuf,"%s/%s",p-len,name);
        *p=s;
        if (*p)
            p++;

        if (access(szbuf,X_OK) == 0)
            found=1;
    }
    if (found == 0)
        found=(-1);

    if (found == 1) /* found */
        found=0;

    return(found);
}

/* eat white space from a open FILE pointer */
/* return -1 if fp is NULL */
int mutilsEatWhitespace(FILE *fp)
{
    int
        c;

    if (fp == NULL)
        return(-1);

    for(c =getc(fp);isspace(c) && ('\n' != c); c=getc(fp))
        ;
    return (c);

}

/* eat comments space from a open FILE pointer */
/* this function is called after seeing the comment character, so the */
/* function actually eats the line till sees a new line */
/* return -1 if fp is NULL */
int mutilsEatComment(FILE *fp)
{
    int
        c;

    if (fp == NULL)
        return(-1);

    for(c=getc(fp); ('\n' != c) && (EOF != c ) && (c > 0); c=getc(fp))
            ;

    return(c);
}

/*
** mutilsGgetDirname()
**   returns the base directory of file (Unix or Windows)
**   if fails returns NULL
**  Note: returns pointer to a malloc'd space, caller is responsible to
**        free it. 
*/
char *mutilsGetDirname(file)
char
    *file;
{
    char
        *f,
        *p;

    if (file == NULL || *file == '\0')
        return (NULL);

    f=strdup(file);
    if (f == NULL)
        return(NULL);

    if ((p=strrchr(f,'/')) || (p=strrchr(f,'\\')))
    {
        *p='\0';
        if (f)
            return(f);
    }
    else
    {
        p=strdup("./");
        return(p);
    }
    return (NULL);
}

/**
 * @brief   Free memory associated with tokens
 * @param tokens    The tokens to free
 * @param ntokens   Number of tokens in tokens
 */
void mutilsFreeTokens(char **tokens,int ntokens)
{
    int
        i;

    /* free memory allocated for each token */
    for (i=0; i < ntokens; i++)
    {
        if (tokens[i])
            (void) free((char *) tokens[i]);
    }

    /* free the tokens itself */
    if (tokens)
        (void) free((char *) tokens);
}

/**
 * @brief Tokenizes a string separated by delimiter
 * @param str       The string to tokenize
 * @param delip     The delimeter e.g. ' ' 
 * @param ntoken    Number of tokens in the string (returns)
 * @return tokens   on success, NULL on failure
 *
 * Example:
 *  int n=0; 
 *  char **tokens=mutils_tokenize("this is"," ",&n);
 * 
 * The caller should free the tokens by calling:
 * mutils_free_tokens(tokens,ntokens)
 * Note: A token can be of MUTILS_MAX_TOKEN_LEN long
 */
char **mutilsTokenize(char *str,int delim,int *ntokens)
{
    char
        tbuf[MUTILS_MAX_TOKEN_LEN],
        **tokens=NULL;

    int
        j=0,
        count=0,
        allocated=0,
        i=0;

    char
        *p;

    *ntokens=0;
    if (str == NULL || *str == '\0')
        return(NULL);

    /* count how many token there first */
    for (p=str; *p; p++)
    {
        if (*p == delim)
        {
            count++;
        }
    }
    count++;
    /* allocate memory for tokens */
    tokens=(char **) malloc(count * sizeof(char *));
    MUTILS_CHECK_MALLOC(tokens);

    allocated=0;
    for (i=0; i < count; i++)
    {
        /*
        ** allocate memory for each token string, a token can 
        ** be of MUTILS_MAX_TOKEN_LEN characters long maximum
        */
        tokens[i]=(char *) malloc(MUTILS_MAX_TOKEN_LEN*sizeof(char));
        MUTILS_CHECK_MALLOC(tokens[i]);
        allocated++;
    }

    j=0;
    count=0;
    for (p=str; *p; p++)
    {
        if (*p != delim && *p != '\0')
        {
            if (j >= MUTILS_MAX_TOKEN_LEN)
            {
                (void) fprintf(stderr,"Buffer overflow detected\n");
                /* buffer overfow */
                goto ExitProcessing;
            }
            tbuf[j++]=*p;
        }
        else
        {
            /* we're in a new token */
            tbuf[j++]='\0';
            strcpy(tokens[count],tbuf);
            count++;
            j=0;
        }
    }
    if (j > 0 && j < MUTILS_MAX_TOKEN_LEN)
    {
        tbuf[j]='\0';
        mutilsStrncpy(tokens[count],tbuf,MUTILS_MAX_TOKEN_LEN-1);
    }

    count++;

    *ntokens=count;
    return(tokens);

ExitProcessing:
    mutilsFreeTokens(tokens,allocated);

    return(NULL);
}

static void *_fs_get (size_t size)
{
    void *block =(void *)  malloc (size ? size : (size_t) 1);
    return (block);
}

static void _fs_give (void **block)
{
    free (*block);
    *block = NULL;
}


/**
 * @brief   encode content to base64
 * @param   src     pointer to source
 * @param   srcl    Length of the source
 * @param   len     length of base64 encoded string (returns)
 * 
 * @return  Ponter to encoded strin gon success, NULL on failure. The
*           caller is responsile to free the memory
 *
 * Adapted from from c-client source
 */
unsigned char *mutils_encode_base64(void *src,unsigned long srcl,unsigned long *len)
{
    unsigned char
        *ret,
        *d;

    unsigned char *s = (unsigned char *) src;
    char *v="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    unsigned long i = ((srcl + 2) / 3) * 4;
    *len = i += 2 * ((i / 60) + 1);
    d = ret = (unsigned char *) _fs_get ((size_t) ++i);
    if (d == NULL)
    {
        (void) fprintf(stderr,"%s (%d) - memory allocation failed\n",MUTILS_CFL);
        return(NULL);
    }
    /* process tuplets */
    for (i = 0; srcl >= 3; s += 3, srcl -= 3)
    {
        *d++ = v[s[0] >> 2]; /* byte 1: high 6 bits (1) */
        /* byte 2: low 2 bits (1), high 4 bits (2) */
        *d++ = v[((s[0] << 4) + (s[1] >> 4)) & 0x3f];
        /* byte 3: low 4 bits (2), high 2 bits (3) */
        *d++ = v[((s[1] << 2) + (s[2] >> 6)) & 0x3f];
        *d++ = v[s[2] & 0x3f]; /* byte 4: low 6 bits (3) */
        if ((++i) == 15)
        {  /* output 60 characters? */
            i = 0;   /* restart line break count, insert CRLF */
            *d++ = '\015';
            *d++ = '\012';
        }
    }
    if (srcl)
    {
        *d++ = v[s[0] >> 2]; /* byte 1: high 6 bits (1) */
        /* byte 2: low 2 bits (1), high 4 bits (2) */
        *d++ = v[((s[0] << 4) + (--srcl ? (s[1] >> 4) : 0)) & 0x3f];
        /* byte 3: low 4 bits (2), high 2 bits (3) */
        *d++ = srcl ? v[((s[1] << 2) + (--srcl ? (s[2] >> 6) : 0)) & 0x3f] : '='; /* byte 4: low 6 bits (3) */
        *d++ = srcl ? v[s[2] & 0x3f] : '=';
        if (srcl)
            srcl--;  /* count third character if processed */
        if ((++i) == 15)
        {  /* output 60 characters? */
            i = 0;   /* restart line break count, insert CRLF */
            *d++ = '\015';
            *d++ = '\012';
        }
    }
    *d++ = '\015';
    *d++ = '\012'; /* insert final CRLF */
    *d = '\0';   /* tie off string */
    if (((unsigned long) (d - ret)) != *len)
    {
        (void) fprintf(stderr,"%s (%d) - logic flow in encode base64\n",MUTILS_CFL);
        (void) free((char *) ret);
        return(NULL);
    }
    return (ret);   /* return the resulting string */
}


/**
 * @brief   converts base64 contents to binary
 * @param   src     The source
 * @param   srcl    Length of the source
 * @param   len     length of converted binary content (returns)
 * 
 * @return  Pointer to biary destination on success, NULL on failure. The
 *          caller is responsible to free ths memory.
 *
 * Adapted from c-client source
 */
void *mutils_decode_base64(unsigned char *src,unsigned long srcl,unsigned long *len)
{
    char c,*s,tmp[BUFSIZ];
    void *ret = _fs_get ((size_t) (*len = 4 + ((srcl * 3) / 4)));
    char *d = (char *) ret;
    int e;
#define JNK 0177
#define PAD 0100
    static char decode[256] = {
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,076,JNK,JNK,JNK,077,
      064,065,066,067,070,071,072,073,074,075,JNK,JNK,JNK,PAD,JNK,JNK,
      JNK,000,001,002,003,004,005,006,007,010,011,012,013,014,015,016,
      017,020,021,022,023,024,025,026,027,030,031,JNK,JNK,JNK,JNK,JNK,
      JNK,032,033,034,035,036,037,040,041,042,043,044,045,046,047,050,
      051,052,053,054,055,056,057,060,061,062,063,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,
      JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK,JNK
     };

    if (ret == NULL)
    {
        (void) fprintf(stderr,"%s (%d) - memory allocation failed\n",MUTILS_CFL);
        return(NULL);
    }
    memset (ret,0,(size_t) *len); /* initialize block */
    *len = 0;   /* in case we return an error */
    
    /* simple-minded decode */
    for (e = 0; srcl--; )
        switch (c = decode[*src++])
        {
            default:   /* valid BASE64 data character */
            switch (e++)
            {  /* install based on quantum position */
                case 0:
                *d = c << 2;  /* byte 1: high 6 bits */
                break;
                case 1:
                *d++ |= c >> 4;  /* byte 1: low 2 bits */
                *d = c << 4;  /* byte 2: high 4 bits */
                break;
                case 2:
                *d++ |= c >> 2;  /* byte 2: low 4 bits */
                *d = c << 6;  /* byte 3: high 2 bits */
                break;
                case 3:
                *d++ |= c;  /* byte 3: low 6 bits */
                e = 0;   /* reinitialize mechanism */
                break;
            }
            break;
            case JNK:   /* junk character */
            break;
            case PAD:   /* padding */
            switch (e++)
            {  /* check quantum position */
                case 3:   /* one = is good enough in quantum 3 */
                /* make sure no data characters in remainder */
                for (; srcl; --srcl)
                    switch (decode[*src++])
                    {
                        case JNK:
                        case PAD: /* ignore junk and extraneous padding */
                        break;
                        default:   /* valid BASE64 data character */
                        /* This indicates bad MIME.  One way that it can 
                           be caused is if a single-section message was 
                           BASE64 encoded and then something (e.g. a 
                           mailing list processor) appended text.  The 
                           problem is that in 1 out of 3 cases, there is 
                           no padding and hence no way to detect the end 
                           of the data.  Consequently, prudent software 
                           will always encapsulate a BASE64 segment inside 
                           a MULTIPART.*/
                        sprintf (tmp,"Possible data truncation in rfc822_base64(): %.80s", (char *) src - 1);
                        if (s = strpbrk (tmp,"\015\012"))
                            *s = 0;
                        /*mm_log (tmp,PARSE);*/
                        srcl = 1;  /* don't issue any more messages */
                        break;
                    }
                break;
                case 2:   /* expect a second = in quantum 2 */
                if (srcl && (*src == '='))
                    break;
                default:   /* impossible quantum position */
                _fs_give (&ret);
                return(NULL);
            }
            break;
        }
    *len = d - (char *) ret; /* calculate data length */
    return ret;   /* return the string */
}

unsigned char mutils_hex_char_to_bin(char x)
{
    if (x >= '0' && x <= '9')
        return(x - '0');

    x=toupper(x);
    return((x - 'A') + 10);
}
/**
 * convert a hex string to it's binary format.
 * @param   hex_string  - The hex string to convert
 * @param   len         - length of the hex string
 * @param   *olen       - The length of the binary string - returns
 *
 * @return  pointer to a unsigned char holding the binary version of the
 *          hex string. returns NULL on failure. The caller should check
 *          olen (> 0) before using the binary
 *
 * @note    Memory is allocated for the retured unsigned char pointer
 *          The caller is responsible to free it
 *
 * The format of the hex string can be any of:
 *      0xde:ad:be:ef:ca:fe
 *      de:ad:be:ef:ca:fe
 *      de-ad-be-ef-ca-fe
 *      0xde-ad-be-ef-ca-fe
 *      de:ad-be_ef:ca:fe
 *
 * The converted binary value will be: de ad be ef ca fe
 * 
 * It will only convert valid hex strings to binary, for example, if
 * the string is like:  de:gh:ad:ff:bb:kk:cc
 * The binary value will contain: de ad ff bb cc
 * Note, gh and kk are ignored
 */
unsigned char *mutils_hex_to_bin(char *hex_string,int len,int *olen)
{
    int
        j=0;
    int
        bin_len=0,
        n=0,
        i; 

    unsigned char
        value,
        *out,
        space[4];
    
    char
        *cp=hex_string;

    if (! hex_string || ! len)
        return(NULL);

    if ((*cp == '0') && ((*(cp + 1) == 'x') || (*(cp + 1) == 'X')))
    {
        cp += 2;
        n=2;
    }

    /*
    ** allocate half of hex_string as the length of binary.
    ** we may be allocated more than needed as : - etc may be
    ** part of the hex string
    */
    bin_len=(len >> 1);
    if (bin_len <= 0)
        return(NULL);

    out=(unsigned char *) malloc(bin_len*sizeof(unsigned char));
    memset(out,0,bin_len);
    for (i=n; i < len; i += 2)
    {
        if (hex_string[i] == '\n' || hex_string[i] == '\r' ||
            hex_string[i] == ' ' || hex_string[i] == '\t' ||
            hex_string[i] == ':' ||
            hex_string[i] == '-' ||
            hex_string[i] == '_')
        {
            i--;
            continue;
        }
        if (isxdigit(hex_string[i]) && isxdigit(hex_string[i+1]))
        {
            value=(mutils_hex_char_to_bin (hex_string[i]) << 4) & 0xf0;
            value |= (mutils_hex_char_to_bin(hex_string[i+1]) & 0x0f);
            out[j++]=value;
        }
    }
    *olen=j;

    return(out);
}

/**
 * @brief   convert binary to hex
 * @param   input   - binary data
 * @param   len     - length of input data
 * @param   output   - NULL terminated string equivalent in hex
 * @return  length of output string (not including NULL) on success, -1
 *          on failure
 *
 * Note: *output points to a malloc'd space, caller is responsible to
 *          to free it
 *          adapted from net-snmp 5.0.6 code (tools.c)
 */
int mutils_binary_to_hex(unsigned char *input,int len,char **output)
{
    int
        olen;
    char
        *s,
        *op;

    unsigned char
        *ip;

    olen=(len * 2) + 1;
    s=(char *) calloc(1,olen);
    if (s == NULL)
        return (-1);
    op=s;

    ip=input;
    while ((ip - input) < len)
    {
        *op++ = MUTILS_VAL2_HEX((*ip >> 4) & 0xf);
        *op++ = MUTILS_VAL2_HEX(*ip & 0xf);
        ip++;
    }
    *op='\0';
    *output=s;

    return(olen);
}

/**
 * @brief   convert binary to hex
 * @param   in      binary data 
 * @param   in_len  length of input in bytes
 * @param   out     output buffer to hold the converted hex data. in/out. The buffer is
 *                  NULL terminated.
 * @param   out_len bytes of memroy pre allocated in out. out_len must be
 *                  greater than in_len*2
 * @return  length of converted hex buffer on success, -1 on failure
 *
 * Note: this function is similar to mutils_binary_to_hex(), only differece is
 * that in this function, the caller passes the pre-allocated buffer. 
 */
int mutils_binary_to_hex_buf(unsigned char *in,int in_len,char *out,int *out_len)
{
    char
        *op;

    unsigned char
        *ip;

    op=out;
    ip=in;
    if (*out_len < (in_len * 2) + 1)
    {
        *out_len=(-1);
        (void) fprintf(stderr,"%s (%d) - outbuf must have atlease %d bytes of space pre-allocated\n",
            MUTILS_CFL,((in_len * 2) + 1));
        return(-1);
    }

    *out_len=(in_len * 2);
    while ((ip - in) < in_len)
    {
        *op++ = MUTILS_VAL2_HEX((*ip >> 4) & 0xf);
        *op++ = MUTILS_VAL2_HEX(*ip & 0xf);
        ip++;
    }
    *op='\0';
    return(*out_len);
}




#ifdef TEST

int main (int argc,char **argv) 
{
    int
        rc;

    rc=mutilsWhich(argv[1]);
    (void) fprintf(stderr,"rc=%d\n",rc);
    return(0);
}
#endif /* TEST */
