#include "mutils.h"
#include <string.h>

int main (int argc,char **argv) 
{
    char
        *s=NULL;
    int
        i;

    if (argc != 2)
    {
        (void) fprintf(stderr,"usage: %s <string>\n",argv[0]);
        exit(1);
    }
    s=strdup(argv[1]);
    (void) fprintf(stderr,"MD5(%s)  %s\n",s,getMD5Digest(s));

    return(0);
}
