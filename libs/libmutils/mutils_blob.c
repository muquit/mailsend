#include "mutils.h"

/* Note: all the blob realated routines are adapted from ImageMagick */

/*
** attaches a blob to the MutilsBlob structure 
*/
void mutils_attach_blob(MutilsBlob *blob_info,void *blob,size_t length)
{
    if (blob == (MutilsBlob *) NULL)
        return;

    blob_info->data=(unsigned char *) blob;
    blob_info->length=length;
    blob_info->offset=0;
}

/* duplicate the given MutilsBlob structure */
/* returns NULL on failure */
/**
 * @brief   Duplicates the given blob
 * @param   blob Blob to duplicate
 * @return  blob The duplicated blob on success, NULL on failure
 *
 */
MutilsBlob *mutils_clone_blobinfo(MutilsBlob *blob)
{
    MutilsBlob
        *clone_info;

    clone_info=(MutilsBlob *) mutils_acquire_memory(sizeof(MutilsBlob));
    if (clone_info == (MutilsBlob *) NULL)
    {
        mutils_error("%s (%d) - unable to clone MutilsBlob, memory allocation failed\n",MUTILS_CFL);
        return(NULL);
    }
    if (blob == (MutilsBlob *) NULL)
    {
        memset(clone_info,0,sizeof(MutilsBlob));
        return(clone_info);
    }

    *clone_info=(*blob);
    return(clone_info);
}

/**
 * @brief rewind a blob
 * @param blob
 *
 * Rewinds a blob after processing it
 */
void mutils_rewind_blob(MutilsBlob *blob)
{
    if (!blob)
        return;
    blob->offset=0;
}

/**
 * allocate memory for blob and and the data member
 * @param data_len - the length of the data in bytes
 * @return - pointer to blob on SUCCESS, NULL on failure
 */
MutilsBlob *mutils_allocate_blob(int data_len)
{
    MutilsBlob
        *blob=NULL;

    if (data_len <= 0)
        return(NULL);

    /* allocate memory for blob */
    blob=mutils_clone_blobinfo(NULL);
    if (blob == NULL)
        return(NULL);

    /* allocate memory for data */
    blob->length=data_len;
    blob->data=(unsigned char *) mutils_acquire_memory(data_len+1);
    if (blob->data == NULL)
    {
        mutils_destroy_blob(blob);
        return(NULL);
    }

    return(blob);
}
 

/*
** detaches a blob from the BlobInfo structure.
*/
void mutils_detach_blob(MutilsBlob *blob)
{
    if (blob == NULL)
        return;

    blob->length=0;
    blob->offset=0;
    blob->data=(unsigned char *) NULL;
}

/**
 * creates a MutilsBlob object. allocates memory for data, copies the passed
 * data, update the length and returns the blob
 * @param data The data to copy to blob's data member
 * @param data_len The number of bytes of data 
 * @return MutilsBlob on success NULL on failure
 */
MutilsBlob *mutils_data_to_blob(unsigned char *data,int data_len)
{
    MutilsBlob
        *blob;

    if (data == (unsigned char *) NULL)
        return(NULL);

    blob=mutils_clone_blobinfo(NULL);
    if (blob == (MutilsBlob *) NULL)
    {
        (void) fprintf(stderr,"%s (%d) unable to create blob, memory allocation problem\n",__FILE__,__LINE__);
        return((MutilsBlob *) NULL);
    }
    blob->length=data_len;
    blob->data=(unsigned char *) mutils_acquire_memory(blob->length+1);
    if (blob->data == NULL)
    {
        (void) fprintf(stderr,"Unable to create blob data, memory allocation problem\n");
        mutils_destroy_blob(blob);
        return((void *) NULL);
    }

    memcpy(blob->data,data,blob->length);

    return(blob);
}

/*
** returns the contents of a file as a blob.
** NULL on failure.
*/
MutilsBlob *mutils_file_to_blob(char *filename)
{
    MutilsBlob 
        *blob;

    long
        count=0;

    size_t
        length;

    int
        fd;

    struct stat
        statbuf;

    if (filename == NULL)
        return(NULL);

    fd=open(filename,O_RDONLY | O_BINARY);
    if (fd == -1)
    {
        (void) fprintf(stderr,"Unable to open file %s\n",filename);
        return((void *) NULL);
    }

    length=(size_t) (fstat(fd,&statbuf)) < 0 ? 0 : statbuf.st_size;
    blob=mutils_clone_blobinfo(NULL);
    if (blob == (MutilsBlob *) NULL)
    {
        (void )close(fd);
        (void) fprintf(stderr,"unable to create blob, memory allocation problem\n");
        return((MutilsBlob *) NULL);
    }
    blob->length=length;
    blob->data=(unsigned char *) mutils_acquire_memory(blob->length+1);
    if (blob->data == NULL)
    {
        (void) fprintf(stderr,"Unable to create blob data, memory allocation problem\n");
        mutils_destroy_blob(blob);
        return((void *) NULL);
    }

    count=read(fd,blob->data,length);
    (void) close(fd);

    if ((size_t) count != length)
    {
        mutils_destroy_blob(blob);
        return((void *) NULL);
    }

    return(blob);
}

/* 
** deallocates memory associated with an BlobInfo structure
*/
void mutils_destroy_blob(MutilsBlob *blob)
{
    if (blob == NULL)
        return;
    
    if (blob->data)
        (void) free(blob->data);
    mutils_liberate_memory((void **) &blob);
}

/*
** converts a least-significant byte first buffer of integers to
** most-significant byte first.
*/
void mutils_msb_order_long(char *p,const size_t length)
{
    register char
    c,
    *q,
    *sp;

    if (p == NULL)
        return;

    q=p+length;
    while (p < q)
    {
        sp=p+3;
        c=(*sp);
        *sp=(*p);
        *p++=c;
        sp=p+1;
        c=(*sp);
        *sp=(*p);
        *p++=c;
        p+=2;
    }
}

/*
** converts a least-significant byte first buffer of integers to
** most-significant byte first.
*/
void mutils_msb_order_short(char *p,const size_t length)
{
    register char
        c,
        *q;

    if (p == NULL)
        return;

    q=p+length;
    while (p < q)
    {
        c=(*p);
        *p=(*(p+1));
        p++;
        *p++=c;
    }
}


/*
** reads data from the blob and returns it. It returns the number of bytes
** read.
**  blob - MutilsBlob
**  length  - number of bytes to read from blob
**  data    - returns
**
**  returns > 0 on sucess, -1 on failure
*/  
int mutils_read_blob(MutilsBlob *blob,const size_t length,void *data)
{
    int
        count,
        n=0;

    if (blob == (MutilsBlob *) NULL)
        return(-1);

    if (blob->data != (unsigned char *) NULL)
    {
        count=MUTILS_MIN(length,blob->length - blob->offset);
        if (count > 0)
        {
            (void) memcpy(data,blob->data + blob->offset,count);
            blob->offset += count;
        }
        /*
        if (count < length)
            return(-1);
        */
        return(count);
    }
    
    return(-1);
}   

/*
** reads a single byte from blob and returns it.
** returns -1 on error
*/
int mutils_read_blob_byte(MutilsBlob *blob)
{
    size_t
        count;

    unsigned char
        buffer[1];

    if (blob == NULL)
        return(-1);

    count=mutils_read_blob(blob,1,(unsigned char *) buffer);
    if (count == 0)
        return(-1);

    return(*buffer);
}

/*
** reads a long value as a 32 bit quantity in least-significant byte first
**order.
*/
unsigned long mutils_read_blob_lsb_long(MutilsBlob *blob)
{
    unsigned char
        buffer[4];

    unsigned long
        value;

    if (blob == NULL)
        return ((unsigned long) ~0);

    value=mutils_read_blob(blob,4,(unsigned char *) buffer);
    if (value == 0)
        return ((unsigned long) ~0);

    value=buffer[3] << 24;
    value|=buffer[2] << 16;
    value|=buffer[1] << 8;
    value|=buffer[0];
    return(value);
}

/*
** reads a short value as a 16 bit quantity in least-significant byte first
* order.
*/
unsigned short mutils_read_blob_lsb_short(MutilsBlob *blob)
{
    unsigned char
        buffer[2];

    unsigned short
        value;

    if (blob == (MutilsBlob *) NULL)
        return((unsigned short) ~0);

    value=mutils_read_blob(blob,2,(unsigned char *) buffer);
    if (value == 0)
        return((unsigned short) ~0);

    value=buffer[1] << 8;
    value|=buffer[0];

    return(value);
}

/**
 * @brief reads a long value as a 32 bit quantity in most-significant byte 
 * firstorder.
 * @param blob      The blob
 * @param err_no    If no error err_no is set to 0 other wise it is set to
 *                  -1
 * @param The read value as unsigned long
 *
 * The caller should always check the err_no instead of the return code,
 * because there's no other way to return a number in case of error
 */
unsigned long mutils_read_blob_msb_long(MutilsBlob *blob,int *err_no)
{
    unsigned char
        buffer[4];

    unsigned long
        value;

    *err_no=0;

    if (blob == (MutilsBlob *) NULL)
    {
        mutils_error("%s (%d) - mutils_read_blob_msb_long() empty blob\n",
                MUTILS_CFL);
        *err_no=(-1);
        return((unsigned long) ~0);
    }

    value=mutils_read_blob(blob,4,(unsigned char *) buffer);
    if (value == 0)
    {
        mutils_error("%s (%d) - mutils_read_blob_msb_long() could not read 4 bytes from blob\n",MUTILS_CFL);
        *err_no=(-1);
        return((unsigned long) ~0);
    }
    /*
    ** We can not return ~0 on error because if the value ffffff  is read,
    ** the return code will indicate it's an error
    */
/*        return((unsigned long) ~0);*/

    value=(unsigned long) buffer[0]  << 24;
    value|=buffer[1] << 16;
    value|=buffer[2] << 8;
    value|=buffer[3];

    return(value);
}

/**
 * @brief reads a short value as a 16 bit quantity in most-significant byte 
 * firstorder.
 * @param blob      The blob
 * @param err_no    If no error err_no is set to 0 other wise it is set to
 *                  -1
 * @return The read value as unsigned short
 *
 * The caller should always check the err_no instead of the return code,
 * because there's no other way to return a number in case of error
 */
unsigned short mutils_read_blob_msb_short(MutilsBlob *blob,int *err_no)
{
    unsigned char
        buffer[2];

    unsigned short
        value;

    *err_no=0;
    if (blob == NULL)
    {
        mutils_error("%s (%d) - mutils_read_blob_msb_short() empty blob\n",
                MUTILS_CFL);
        *err_no=(-1);
        return((unsigned short) ~0);
    }

    value=mutils_read_blob(blob,2,(unsigned char *) buffer);
    if (value == 0)
    {
        mutils_error("%s (%d) - mutils_read_blob_msb_short() could not read 2 bytes from blob\n",MUTILS_CFL);
        *err_no=(-1);
        return((unsigned long) ~0);
    }

    /*
    ** We can not return ~0 on error because if the value ffff is read,
    ** the return code will indicate it's an error
    */
/*        return((unsigned short) ~0);*/

    value=(unsigned short) (buffer[0] << 8);
    value |= buffer[1];

    return(value);
}

/* reads characters from a blob until a new line or cr is read*/
/* the string is null terminated */
char *mutils_read_blob_string(MutilsBlob *blob,char *string,int slen)
{
    int
        c,
        i;

    if (blob == NULL)
        return(NULL);

    for (i=0; i < slen; i++)
    {
        c=mutils_read_blob_byte(blob);
        if (c == -1)
            return((char *) NULL);
        string[i]=c;
        if ((string[i] == '\n') || (string[i] == '\r'))
            break;
    }
    string[i]='\0';
    return(string);
}

/* returns current size of the blob */
/* -1 on error */
int  mutils_size_blob(MutilsBlob *blob)
{
    if (blob == NULL)
        return(-1);

    if (blob->data != (unsigned char *) NULL)
        return(blob->length);

    return(-1);
}

/* returns the current value of the blob position */
/* -1 on failure */
int mutils_tell_blob(MutilsBlob *blob)
{
    if (blob == NULL)
        return(-1);

    if (blob->data != (unsigned char *) NULL)
        return(blob->offset);

    return(-1);
}

/**
 * @brief   Writes length bytes of data to blob
 * @param   blob    The blob to fill with data
 * @param   length  Length of data
 * @param   data    Data to write to blob
 *
 * blob->data must have at least blob->length bytes of data pre-allocated.
 * If we need more than that, memory will be allocated dynamically.
 *
 * blob->offset is incremented with the amount of data written to blob
 *
 * This code is adapted from ImageMagick. ImageMagick's version reallocate
 * data bultiple of 8 bytes. This one allocates exact number of bytes as 
 * needed.
 *
 */
int mutils_write_blob(MutilsBlob *blob,const size_t length,const void *data)
{
    int
        reallocate_bytes=0;

    if (blob == (MutilsBlob *) NULL)
        return(-1);
    
    if (blob->data != (unsigned char *) NULL)
    {
        if (length > (blob->length - blob->offset))
        {
            reallocate_bytes=(length - (blob->length - blob->offset));
            mutils_info("%s (%d) out of memory, re-allocating %d bytes\n",
                    MUTILS_CFL,reallocate_bytes);
            /* find out how many bytes we need to re-allocate */
            blob->length += reallocate_bytes;
            mutils_reacquire_memory((void **) &blob->data,blob->length);
            if (blob->data == (unsigned char *) NULL)
            {
                mutils_detach_blob(blob);
                return(-1);
            }
        }
        (void) memcpy(blob->data + blob->offset,data,length);
        blob->offset += length;
        if (blob->offset > (off_t) blob->length)
            blob->length=blob->offset;
        return(length);
    }

    return(-1);
}

/*
** write an integer to a blob. It returns the number of bytes written 
** and -1 on failure
*/
size_t mutils_write_blob_byte(MutilsBlob *blob,const long value)
{

    unsigned char
        buffer[1];

    if (blob == NULL)
        return(-1);

    buffer[0]=(unsigned char) value;
    return(mutils_write_blob(blob,1,buffer));
}
/*
** writes a long value as a 32 bit quantity in least-significant byte first
** order. returns the number of unsigned longs written. -1 on error
*/
int mutils_write_blob_lsb_long(MutilsBlob *blob,const unsigned long value)
{
    unsigned char
        buffer[4];

    if (blob == NULL)
        return(-1);

    buffer[0]=(unsigned char) value;
    buffer[1]=(unsigned char) (value >> 8);
    buffer[2]=(unsigned char) (value >> 16);
    buffer[3]=(unsigned char) (value >> 24);

    return(mutils_write_blob(blob,4,buffer));
}

/*
** writes a long value as a 16 bit quantity in least-significant byte first
** order. returns the number of unsigned longs written. -1 on failure 
*/
int mutils_write_blob_lsb_short(MutilsBlob *blob,const unsigned long value)
{
    unsigned char
        buffer[2];

    if (blob == NULL)
        return(-1);

    buffer[0]=(unsigned char) value;
    buffer[1]=(unsigned char) (value >> 8);

    return(mutils_write_blob(blob,2,buffer));
}

/*
** writes a long value as a 32 bit quantity in most-significant byte first
** order. returns the number of unsigned longs written. -1 on failure 
*/
int mutils_write_blob_msb_long(MutilsBlob *blob,const unsigned long value)
{
    unsigned char
        buffer[4];

    if (blob == NULL)
        return(-1);

    buffer[0]=(unsigned char) (value >> 24);
    buffer[1]=(unsigned char) (value >> 16);
    buffer[2]=(unsigned char) (value >> 8);
    buffer[3]=(unsigned char) value;

    return(mutils_write_blob(blob,4,buffer));

}

/*
** writes a long value as a 16 bit quantity in most-significant byte first
** order. returns the number of unsigned longs written. -1 on failure 
*/
int mutils_write_blob_msb_short(MutilsBlob *blob,const unsigned long value)
{
    unsigned char
        buffer[2];

    if (blob == NULL)
        return(-1);

    buffer[0]=(unsigned char) (value >> 8);
    buffer[1]=(unsigned char) value;

    return(mutils_write_blob(blob,2,buffer));
}

/*
** write a string to a blob.  It returns the number of characters written
*/  
size_t mutils_write_blob_string(MutilsBlob *blob,const char *string)
{
    if (string == NULL)
        return(0);

    if (blob == NULL)
        return(0);

    return(mutils_write_blob(blob,strlen(string),string));
}



void mutils_hex_dump_blob(FILE *fp,MutilsBlob *blob)
{
    if (blob && blob->data)
    {
        if (blob->length)
            mutils_hex_dump(fp,blob->data,blob->length);
    }
    else
    {
        (void) fprintf(stderr,"Empty blob\n");
    }
}
