#include "mutils.h"

int main (int argc,char **argv) 
{
    int
        rc;
    if (argc != 3)
    {
        (void) fprintf(stderr," usage: %s <string> <mask>\n",argv[0]);
        exit(1);
    }

    rc=mutilsIsinname(argv[1],argv[2]);
    if (rc)
        (void) fprintf(stderr,"%s %s fits\n",argv[1],argv[2]);
    return(0);
}
