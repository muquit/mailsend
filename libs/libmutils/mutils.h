#ifndef MUTILS_H
#define MUTILS_H

#include <stdio.h>

#if STDC_HEADERS || HAVE_STRING_H
#include <string.h> /* ANSI string.h and pre-ANSI memory.h might conflict*/
#if !STDC_HEADERS && HAVE_MEMORY_H
#include <memory.h>
#endif
#else
#if  HAVE_STRINGS_H
#include <strings.h>
#endif
#endif


#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#if SYS_UNIX
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef WINNT
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <share.h>
#include <direct.h>
#include <Winsock2.h> /* for timeval */
#define ftruncate chsize

#ifdef getcwd
#undef getcwd
#endif
#define getcwd _getcwd

#ifdef snprintf
#undef snprintf
#endif
#define snprintf _snprintf

#endif

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if HAVE_STDLIB_H 
#include <stdlib.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <stdarg.h>

#if HAVE_FCNTL_H
#ifndef O_RDONLY    /* prevent multiple inclusion on lame systems (from
vile)*/
#include <fcntl.h>
#endif
#endif

#if HAVE_MALLOC_H
#include <malloc.h>
#endif


#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if APR_HAVE_LIMITS_H
#include <limits.h>
#else
#if APR_HAVE_SYS_SYSLIMITS_H
#include <sys/syslimits.h>
#endif
#endif

#if defined(PATH_MAX)
#define MUTILS_PATH_MAX       PATH_MAX
#elif defined(_POSIX_PATH_MAX)
#define MUTILS_PATH_MAX       _POSIX_PATH_MAX
#else
#define MUTILS_PATH_MAX  4098
#endif

#if !defined(O_BINARY)
#define O_BINARY  0x00
#endif

#define MUTILS_TRUE   1
#define MUTILS_FALSE  0


typedef struct _MutilsBlob
{
    size_t
        length;

    unsigned char
        *data;

    size_t
        offset,
        size;
}MutilsBlob;

#ifdef WINNT
typedef struct _MutilsTime
{
    FILETIME
        absolute;
}MutilsTime;
#else
typedef struct _MutilsTime
{
    unsigned int
        secs,
        nsecs;
}MutilsTime;
#endif /* WINNT */


#define MUTILS_CFL  __FILE__,__LINE__
#define MCFL             __FILE__,__LINE__
#define MJL              __LINE__

#if __STDC__ || defined(sgi) || defined(_AIX)
#undef _Declare
#define _Declare(formal_parameters) formal_parameters
#else
#define _Declare(formal_parameters) ()
#define const
#endif

#define MUTILS_MAX_TOKEN_LEN     1024

#define MUTILS_CHECK_MALLOC(p) \
do \
{ \
    if (p == NULL) \
    {\
        (void) fprintf(stderr,"%s (%d) - memory allocation problem\n",__FILE__,__LINE__); \
        goto ExitProcessing; \
    }\
}while(0)

/* from net-snmp 5.0.6 tools.h */
#define MUTILS_FREE(s)  if (s) { free((void *)s); s=NULL;}
#define MUTILS_TOUPPER(c)  (c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c)
#define MUTILS_TOLOWER(c)  (c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c)

#define MUTILS_VAL2_HEX(s)  ( (s) + (((s) >= 10) ? ('a'-10) : '0') )
#define MUTILS_HEX2_VAL(s)  \
        ((isalpha(s) ? (MUTILS_TOLOWER(s)-'a'+10) : (MUTILS_TOLOWER(s)-'0')) & 0xf)

#define MUTILS_MAX(a,b) ((a) > (b) ? (a) : (b))
#define MUTILS_MIN(a,b) ((a) > (b) ? (b) : (a))
/* from net-snmp 5.0.6 tools.h */




/* function prototypes */
typedef void (*mutils_error_func) (char *fmt,va_list args);
typedef void (*mutils_hexdump_func) (unsigned char *bytes,int bytes_len);
void mutils_error (char *fmt,...);
void mutils_info (char *fmt,...);
void mutils_warn (char *fmt,...);
void mutils_debug(char *fmt,...);
void mutils_set_debug(int debug);
void mutils_set_error_hook(mutils_error_func new_func);
void mutils_set_info_hook(mutils_error_func new_func);
void mutils_set_debug_hook(mutils_error_func new_func);
void mutils_set_warn_hook(mutils_error_func new_func);
void mutils_set_hexdump_hook(mutils_hexdump_func new_func);
void mutils_set_hexprint_hook(mutils_hexdump_func new_func);

void mutils_reset_error_hook(void);
void mutils_reset_info_hook(void);
void mutils_reset_warn_hook(void);
void mutils_reset_debug_hook(void);

void  mutilsBase64Encode    (FILE *ifp,FILE *ofp);
void  mutilsGenerateMIMEBoundary(char *boundary,int len);
int   mutilsParseURL        (char *url,char *hostname, 
                             int hostname_len, int *port,
                             char *page,int page_len);

void  mutilsStripLeadingSpace  (char *s);
void  mutilsStripTrailingSpace (char *s);
char  *mutilsReverseString  (char *str);
char  *mutilsStrncat        (char *dst,char *src,int n);
char  *mutilsStrncpy        (char *dsr,char *src,int n);
int   mutilsStrncasecmp     (char *s1,char *s2,int n);
char  *mutilsStrdup         (char *str);
int   mutilsStrcasecmp      (char *a,char *b);
void  mutilsSafeStrcpy      (char *dst,char *src,int n);
void  mutilsSafeStrcat      (char *dsr,char *src,int n,int ss,int sl);
char  *mutilsStrtok         (char *s,char *delim);
int   mutilsHowmanyCommas   (char *buf);
void  mutilsCommaize        (char *buf);
void  mutilsCleanBuf        (char *buf,int bufsize,int *length);
char  *mutilsRmallws        (char *str);
char  *mutilsStristr        (char *s,char *t);
int   mutilsIsinname        (char *string,char *mask);
char  *mutilsGetTime        (void);
char  mutilsChopNL          (char *str);
/*int  mutilsTmpFilename     (char *filename);*/
char  *mutilsBasename       (char *path);
char  *mutilsExtension      (char *file_path);
char  *mutilsExtensionLower (char *file_path);
int   mutilsWhich           (char *name);
void  mutilsSetLock         (int fd);
void  mutilsDotLock         (char *filepath,char *errbuf);
void  mutilsDotUnlock       (int delete);
char  *mutilsStrUpper       (char *str);
char  *mutilsStrLower       (char *str);
int   mutilsEatComment      (FILE *fp);
int   mutilsEatWhitespace   (FILE *fp);
char  *mutilsGetDirname     (char *file);
char  *mutilsSpacesToChar   (char *str,int c);
char  **mutilsTokenize(char *str,int delim,int *ntokens);
void  mutilsFreeTokens(char **tokens,int ntokens);
int   mutils_base64_encode_len(int len);
unsigned char *mutils_encode_base64(void *src,unsigned long srcl,unsigned long *len);
char *mutils_encode_base64_noformat(const char *src, int src_len);
void *mutils_decode_base64(unsigned char *src,unsigned long srcl,unsigned long *len);

int mutils_make_encrypted_key(const char *plain,unsigned char *cipher,int clen);
int mutils_decrypt_key(unsigned char *cipher,int clen,char *plain,int plain_buf_len);
char *mutils_decrypt_hex(char *hex,int hex_len,char *pbuf,int pbuf_len);
void mutils_hex_dump(FILE *fp,unsigned char *buf,int length);
void mutils_hex_print(FILE *fp,unsigned char *bytes,int len);
unsigned char *mutils_get_master_key24(void);
unsigned char *mutils_get_iv8(void);

unsigned char mutils_hex_char_to_bin(char x);
unsigned char *mutils_hex_to_bin(char *hex_string,int len,int *olen);
int mutils_binary_to_hex(unsigned char *input,int len,char **output);
int mutils_binary_to_hex_buf(unsigned char *in,int in_len,char *out,int *out_len);

void *mutils_acquire_memory(size_t size);
void mutils_liberate_memory(void **memory);
void mutils_reacquire_memory(void **memory,const size_t size);
void     mutils_attach_blob(MutilsBlob *blob_info,void *blob,size_t length);
void mutils_rewind_blob(MutilsBlob *blob);
MutilsBlob *mutils_clone_blobinfo(MutilsBlob *blob_info);
MutilsBlob *mutils_allocate_blob(int data_len);
MutilsBlob *mutils_file_to_blob(char *filename);
MutilsBlob *mutils_data_to_blob(unsigned char *data,int data_len);
void     mutils_detach_blob(MutilsBlob *blob_info);
void     mutils_destroy_blob(MutilsBlob *blob);
void     mutils_msb_order_long(char *p,const size_t length);
void     mutils_msb_order_short(char *p,const size_t length);

int     mutils_read_blob(MutilsBlob *blob_info,const size_t length,
                                void *data);
int            mutils_read_blob_byte(MutilsBlob *blob);
unsigned long  mutils_read_blob_lsb_long(MutilsBlob *blob_info);
unsigned short mutils_read_blob_lsb_short(MutilsBlob *blob);
unsigned long  mutils_read_blob_msb_long(MutilsBlob *blob,int *err_no);
unsigned short mutils_read_blob_msb_short(MutilsBlob *blob,int *err_no);
char           *mutils_read_blob_string(MutilsBlob *blob,char *string,int slen);
int     mutils_size_blob(MutilsBlob *blob);
int     mutils_tell_blob(MutilsBlob *blob);

int     mutils_write_blob(MutilsBlob *blob,const size_t length,const void *data);
size_t  mutils_write_blob_byte(MutilsBlob *blob_info,const long value);
int     mutils_write_blob_lsb_long(MutilsBlob *blob,const unsigned long value);
int     mutils_write_blob_lsb_short(MutilsBlob *blob,const unsigned long value);
int     mutils_write_blob_msb_long(MutilsBlob *blob,const unsigned long value);
int     mutils_write_blob_msb_short(MutilsBlob *blob,const unsigned long value);
size_t  mutils_write_blob_string(MutilsBlob *blob_info,const char *string);
char    *mutils_getcwd(char *buf,int buflen);
FILE    *mutils_get_tempfileFP(char *tempfile_path,int buflen);
char    *mutils_basename(const char *path);

/* time */
int mutils_time_now(MutilsTime *mt);
void mutils_time_fmt(MutilsTime *mt,char *buf,int bufsiz);


#endif /* MUTILS_H */
