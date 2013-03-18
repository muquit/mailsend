/* all interesting strngs related routines */

#include <mutils.h>


/*
**  reverseString()
**  reverse a string
**
**  Parameters:
**  char    *str    string to modify
**
**  Return Values:
**  pointer to the modified string
**
**  Limitations and Comments:
**  str is modified
**  borrowed from c-snippets STRREV.C, public domain by Bob Stout
**  The name of the function was stttev() -- muquit, May-26-1999
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   may-26-1999    first cut
*/

char *mutilsReverseString(char *str)
{
    char
        *p1,
        *p2;

    if (! str || ! *str)
        return str;

    for (p1=str,p2 =str+strlen(str)-1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return (str);
}


/*
**  mutilsStrncat()
**  appends at most n characters of src string to dst string
**
**  Parameters:
**      dst     destination string
**      src     source string
**      n       number of character to take from src
**
**
**  Return Values:
**
**
**  Limitations and Comments:
**      taken from Henry Spencer's public domain string library
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
char *mutilsStrncat(char *dst,char *src,int n)
{
    register char
        *dscan,
        *sscan;

    register int
        count;

    for (dscan=dst; *dscan != '\0'; dscan++)
        continue;

    sscan=src;
    count=n;

    while (*sscan != '\0' && --count >= 0)
        *dscan++ = *sscan++;

    *dscan++ = '\0';

    return (dst);
}
/*
**  mutilsStrncpy()
**  copy at most n characters of string src to dst
**
**  Parameters:
**  char    *dst    
**  char    *str
**  int     n
**
**  Return Values:
**  pinter to the destination string
**
**  Limitations and Comments:
**   adapted from henry's stringlib
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
char *mutilsStrncpy(char *dst,char *src,int n)
{
    register char
        *dscan;

    register char
        *sscan;

    register int
        count;

    dscan=dst;
    sscan=src;
    count=n;

    while (--count >= 0 && (*dscan++ = *sscan++) != '\0')
        continue;

    while (--count >= 0)
        *dscan++ = '\0';
    return(dst);
}

/*
**  mutilsStrdup()
**      duplicate a sting
**
**  Parameters:
**      char *string    string to duplicate
**
**  Return Values:
**      pointer to the duplicated stirng. NULL if fails.
**
**  Limitations and Comments:
**      allocate memory, caller is responsible to free it.
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
char *mutilsStrdup(char *string)
{
    char
        *tmp;
    
    if (string == NULL || *string == '\0')
        return ((char *) NULL);

    tmp = (char *) malloc ((int) strlen(string)*sizeof(char)+1 );

    if (tmp == (char *) NULL)
    {
        return ((char *) NULL);
    }
    /* it's safe to copy this way */
    (void) strcpy(tmp, string);
    return (tmp);
}


/*
**  mutilsStrcasecmp()
**  case insensitive string comparison
**
**  Parameters:
**  char    *a  
**  char    *b
**
**  Return Values:
**  < 0 if a < b
**  > 0 if a > b
**  0   if a = b
**
**  Limitations and Comments:
**  found somewhere on the net. I didn't write it
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
int mutilsStrcasecmp(char *a,char *b)
{
    register char
        ac,
        bc;

    for(;;)
    {
        ac = *a++;
        bc = *b++;

        if(ac == 0)
        {
            if(bc == 0)
                return 0;
            else
                return -1;
        }
        else
        {
            if(bc == 0)
                return 1;
            else
            {
                if(ac != bc)
                {
                    if(islower(ac)) ac = toupper(ac);
                    if(islower(bc)) bc = toupper(bc);
                    if( ac != bc )
                        return ac - bc;
                }
            }
        }
    }

}

/*
**  mutilsStrncasecmp()
**  case in-sensitive comparison wth first n bytes of fist string
**
**  Parameters:
**  char    *s1
*   char    *s2
*   int     n
**
**  Return Values:
**
**
**  Limitations and Comments:
**  < 0 if first n bytes of a < b
**  > 0 if first n bytes of a a > b
**  0   if first n bytes of a a = b
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Jul-31-1999    first cut
*/
int mutilsStrncasecmp(char *s1,char *s2,int n)
{
    register char
        *scan1,
        *scan2;

    int
        count;

    scan1=s1;
    scan2=s2;
    count=n;

    while (--count >= 0 && *scan1 != '\0' && tolower(*scan1) == tolower(*scan2))
    {
        scan1++;
        scan2++;
    }
    if (count < 0)
        return (0);


    return(tolower(*scan1) - tolower(*scan2));
}

/*
**  mutilsSafeStrcpy()
**  copy a string to another safely without overflowing buffer
**
**  RCS
**      $Revision: 1 $
**      $Date: 2/24/04 8:38p $
**  Return Values:
**      none
**
**  Parameters:
**      dst      destination buffer
**      src    source buffer
**      n      max allowable n of the src
**
**  Side Effects:
**      if copying is safe, dst buffer is modified.
**
**  Limitations and Comments:
**      if the source string is longer than n, it's truncated to
**      n. dst must be static or dynamically allocated buffer, that
**      is it must be able to hold atleast lenth bytes buffer. The src
**      is checked if it is NULL or not. if NULL the routine will exit 
**      after writing an error message image.
**      
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Oct-17-1997    first cut
*/
void mutilsSafeStrcpy(char *dst,char *src,int n)
{
    /* now copy */
    /*
    (void) mutilsStrncpy(dst,src,n);
    */
    (void) strncpy(dst,src,n);
    dst[n]='\0';
}

/*
 *  mutilsSafeStrcat()
 *  safely appends one string to another without overflowing the source
 *  buffer.
 *
 *  Parameters:
 *      dst          appends with this string
 *      src             string to append
 *      length          first this many character of src to append
 *      ssc_size        maximum size of destination string
 *      ssc_length      length of string in destination buffer
 *
 *  Return Values:
 *      none
 *
 *  Limitations and Comments:
 *      dst must have enough space staically or dynamically allocated
 *      before calling. Need to provide a portable strncat() as matt
 *      suggested that some versions of strncat (e.g. solaris 2.5.1) write
 *      at most n+1 characters past the end of s1 and then replace the 
 *      n+1st character with a '\0';
 *
 *
 *  Development History:
 *      who                  when           why
 *      ma_muquit@fccc.edu   Oct-1997       first cut
 *      mhpower@mit.edu      Nov-03-1997    fixed
 */
void mutilsSafeStrcat(char *dst,char *src,int length,int ssc_size,
        int ssc_length)
{
   int copy_length;

   if ( NULL == src )
   {
       /*
       StringImage("Source buffer is NULL in safeStrcat()!");
       */
       (void) fprintf(stderr,"Source buffer is NULL in safeStrcat()!\n");
       exit(0);
   }

   if ((int) strlen(src) >= ssc_size - ssc_length)
   {
       /*
       StringImage("buffer overflow detected! aborting");
       */
       (void) fprintf(stderr,"buffer overflow detected! aborting\n");
       exit(0);
   }

   if (length < ssc_size - ssc_length)
   {
       copy_length = length;
   }
   else
   {
       copy_length = ssc_size - ssc_length;
   }

   /* now copy */
   (void) mutilsStrncat(dst,src,copy_length);

}

/*
 * Copyright (c) 1988, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by the University of
 *  California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
** I'm renaming it to mystrtok() in order to avoid conflict with the
** system which might have it
** I also formatted to my coding style
** 10/08/95, muquit@semcor.com
*/

char *mutilsStrtok(char *s,char *delim)
{
    register char
        *spanp;

    register int
        c,
        sc;

    char
        *tok;

    static char
        *last;


    if (s == (char *) NULL && (s = last) == (char *) NULL)
        return ((char *) NULL);

     /*
     ** Skip (span) leading delimiters (s += strspn(s, delim), sort of).
     */
cont:
    c = *s++;
    for (spanp = (char *)delim; (sc = *spanp++) != 0;)
    {
        if (c == sc)
            goto cont;
    }

    if (c == 0)
    {       /* no non-delimiter characters */
        last = (char *) NULL;
        return ((char *)NULL);
    }
    tok = s - 1;

    /*
     * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
     * Note that delim must have one NUL; we stop if we see that, too.
     */
    for (;;)
    {
        c = *s++;
        spanp = (char *)delim;
        do
        {
            if ((sc = *spanp++) == c)
            {
                if (c == 0)
                    s = (char *) NULL;
                else
                    s[-1] = '\0';
                last = s;
                return (tok);
            }
        } while (sc != 0);
    }
      /* NOTREACHED */
}


/*
** convert a string to upper case
** borrowed from C snippets library (strupr.c)
*/
char *mutilsStrUpper(char *str)
{
    char
        *s;

    if (str)
    {
        for (s=str; *s; ++s)
            *s=toupper(*s);
    }

    return (str);
}

/*
** convert a string to lower case
** borrowed from C snippets library (strupr.c)
*/
char *mutilsStrLower(char *str)
{
    char
        *s;

    if (str)
    {
        for (s=str; *s; ++s)
            *s=tolower(*s);
    }

    return (s);
}
