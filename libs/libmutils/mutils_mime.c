#include "mutils.h"

static char base64_chars[64] =
{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
  'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
  't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', '+', '/'
};



/*
**  generateBoundary()
**   generate unuque string for boundary for MIME tag
**
**  Parameters:
**      char *boundary - NULL terminated boundary string - returns
**      int len        - size of boundary (malloc'd or static)
**
**  Return Values:
**      none
**
**  Limitations and Comments:
**      boundary must have len bytes in it to store the boundary. the
functio
n
**      calls rand() for random number, so the caller should call srand()
**      before calling this function.
**
**      This function is adapted from mutt code.
**
**  Development History:
**      who                  when           why
**      muquit@muquit.com    Mar-06-2002    needs MIME support
*/
#define BOUNDARY_LEN    16
void mutilsGenerateMIMEBoundary(char *boundary,int len)
{
    char
        *p;
    int
        i;
    memset(boundary,0,len);
    p=boundary;
    for (i=0; i < BOUNDARY_LEN; i++)
    {
        if (i >= (len-1))
            break;
        *p++ = base64_chars[rand() % sizeof(base64_chars)];
    }
    *p='\0';
}

static void output64Chunk(int c1,int c2,int c3,int pads, FILE *outfile)
{
    putc(base64_chars[c1>>2], outfile);
    putc(base64_chars[((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)], outfile);
    if (pads == 2)
    {
        putc('=', outfile);
        putc('=', outfile);
    }
    else if (pads)
    {
        putc(base64_chars[((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6)], outfile);
        putc('=', outfile);
    }
    else
    {
        putc(base64_chars[((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6)], outfile);
        putc(base64_chars[c3 & 0x3F], outfile);
    }
}

void mutilsBase64Encode(FILE *ifp,FILE *ofp)
{
    int
        c1,
        c2,
        c3,
        ct=0;

    while ((c1=getc(ifp)) != EOF)
    {
        c2=getc(ifp);
        if (c2 == EOF)
        {
            output64Chunk(c1,0,0,2,ofp);
        }
        else
        {
            c3=getc(ifp);
            if (c3 == EOF)
            {
                output64Chunk(c1,c2,0,1,ofp);
            }
            else
            {
                output64Chunk(c1,c2,c3,0,ofp);
            }
        }
        ct += 4;
        if (ct > 71)
        {
            putc('\r',ofp); /* qmail fix 15.07.05 (movi) */
            putc('\n',ofp);
            ct=0;
        }
    }
    if (ct)
    {
        putc('\r',ofp); /* qmail fix 15.07.05 (movi) */
        putc('\n',ofp);
    }

    (void) fflush(ofp);
}
