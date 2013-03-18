#include "mutils.h"

int main(int argc,char **argv)
{
    char
        buf[BUFSIZ];

    FILE
        *ifp,
        *ofp;

    if (argc != 2)
    {
        (void) fprintf(stderr,"usage: %s file\n",argv[0]);
        return (1);
    }

    (void) sprintf(buf,"%s.b64",argv[1]);
    ifp=fopen(argv[1],"r");
    if (ifp == NULL)
    {
        (void) fprintf(stderr,"could not open for reading: %s\n",argv[1]);
        return (1);
    }

    ofp=fopen(buf,"w");
    if (ofp == NULL)
    {
        (void) fprintf(stderr,"could not open for writing: %s\n",buf);
        return (1);
    }

    mutilsBase64Encode(ifp,ofp);

    (void) fclose(ifp);
    (void) fclose(ofp);


    return (0);
}
