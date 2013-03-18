#include "mutils.h"
#include <errno.h>

/*
** return the path of current working directory 
*/
char *mutils_getcwd(char *buf,int buflen)
{
    if (getcwd(buf,buflen - 1) != NULL)
    {
        return(buf);
    }
    return(NULL);
}

/*
** tmpdir - path of temp directory
** tempfile_path - returns. path of temp filename
** buflen - length of tempfile_path buffer, preallocated
**
** returns: open FILE pointer to temp file
*/
static FILE *open_tempfile(const char *tempdir,char *tempfile_path,int buflen)
{
    FILE
        *fp = NULL;

    int
        rc,
        tfwd,
        len = strlen(tempdir);

    if (len && len < MUTILS_PATH_MAX)
    {
        (void) snprintf(tempfile_path,buflen - 1,"%s/%s",tempdir,"mailsendXXXXXX");
#ifdef HAVE_MKSTEMP
        tfwd = mkstemp(tempfile_path);
        if (tfwd == -1)
        {
            mutils_error("%s (%d) - Could not open tmp file %s for writing: %s\n",
                    MUTILS_CFL,
                    tempfile_path,
                    strerror(errno));
            return(0);
        }

        fp = fdopen(tfwd,"w");
        if (fp == NULL)
        {
            mutils_error("%s (%d) - Could not open temp file %s\n",
                    MUTILS_CFL,
                    tempfile_path);
            return(NULL);
        }
        return(fp);
#else
#ifdef WINNT
        rc = _mktemp_s(tempfile_path, buflen);
        if (rc != 0)
        {
            mutils_error("%s (%d) - Could not create file %s\n",
                    MUTILS_CFL,
                    tempfile_path);
            return(NULL);
        }
        fp = fopen(tempfile_path,"w");
        if (fp == NULL)
        {
            mutils_error("%s (%d) - Could not open temp file %s\n",
                    MUTILS_CFL,
                    tempfile_path);
            return(NULL);
        }
#else
        mktemp(tempfile_path);
        fp = fopen(tempfile_path,"w");
        if (fp == NULL)
        {
            mutils_error("%s (%d) - Could not open temp file %s\n",
                    MUTILS_CFL,
                    tempfile_path);
            return(NULL);
        }

#endif /* WINNT */
#endif  /* HAVE_MKSTEMP */
    }

    return(fp);
}

/*
** Return FILE pointer to a open temp file.
** Parameters:
**  tempfile_path - returns. The buffer to hold the temp file path
**  buflen        - Length of tempfile_path
** Returns:
**   FILE pointer to tempfile on success, NULL on failure
**
** The caller is responsible to close the FILE pointer and remove the file
**
** Note: adapted from apache apr lib
*/
FILE *mutils_get_tempfileFP(char *tempfile_path,int buflen)
{
    char
        tmpbuf[MUTILS_PATH_MAX];

    int
        tfwd;

    FILE
        *fp = NULL;

    char
        *dir = NULL;

    const char *try_envs[] =
    {
        "TMP",
        "TEMP",
        "TMPDIR",
    };

    const char *try_dirs[] =
    {
        "/tmp",
        "/usr/tmp",
        "/var/tmp",
    };

    char 
        *tempdir;

    int
        i;

    /* first look for the env vars */
    for (i = 0; i < (sizeof(try_envs) / sizeof(const char *)); i++)
    {

        tempdir = getenv(try_envs[i]);
        if (tempdir != NULL)
        {
            int 
                len = strlen(tempdir);
            fp = open_tempfile(tempdir,tempfile_path,buflen);
            if (fp != NULL)
                goto ExitProcessing;
        }
    }

    /* we could not find the env vars for temp dir */
    /* try c:/TMP for windows */
#ifdef WINNT
    fp = open_tempfile("c:/TEMP",tempfile_path, buflen);
    if (fp != NULL)
        goto ExitProcessing;
#endif /* WINNT */

    /* Next try some hard coded path */
    for (i = 0; i < (sizeof(try_dirs) / sizeof(const char *)); i++)
    {
        fp = open_tempfile(try_dirs[i], tempfile_path, buflen);
        if (fp != NULL)
            goto ExitProcessing;
    }

    /* Finally, try the current working directory */
    tempdir = mutils_getcwd(tmpbuf,sizeof(tmpbuf)-1);
    if (tempdir != NULL)
    {
        fp = open_tempfile(tempdir, tempfile_path, buflen);
        if (fp != NULL)
            goto ExitProcessing;
    }

ExitProcessing:
    return(fp);
}

#ifdef TESTING
int main(int argc,char *argv[])
{
    char
        *pwd,
        tempfile[MUTILS_PATH_MAX];

    FILE
        *fp;

    fp = mutils_get_tempfileFP(tempfile, MUTILS_PATH_MAX);
    if (fp)
    {
        (void) fprintf(stderr,"%s\n",tempfile);
        (void) fclose(fp);
        unlink(tempfile);
    }
    pwd = mutils_getcwd(tempfile,sizeof(tempfile));
    if (pwd)
        (void) fprintf(stderr,"pwd=%s\n",pwd);

    return(0);
}
#endif
