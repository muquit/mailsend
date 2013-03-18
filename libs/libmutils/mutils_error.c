#include "mutils.h"
/*
** Allow the caller to register a error printing routine
*/

static int s_mutils_debug=0;


static void mutils_default_proc(char *fmt,va_list args)
{
    if (fmt)
    {
        vfprintf(stderr,fmt,args);
        (void) fflush(stderr);
    }
}

static void mutils_default_hexdump_proc(unsigned char *bytes,int bytes_len)
{
    if (bytes && bytes_len > 0)
    {
        mutils_hex_dump(stderr,bytes,bytes_len);
    }
}

static void mutils_default_hexprint_proc(unsigned char *bytes,
        int bytes_len)
{
    if (bytes && bytes_len > 0)
    {
        mutils_hex_print(stderr,bytes,bytes_len);
    }
}

/* default error, info and warning printing routine */
static mutils_error_func mutils_error_hook=mutils_default_proc;
static mutils_error_func mutils_info_hook=mutils_default_proc;
static mutils_error_func mutils_debug_hook=mutils_default_proc;
static mutils_error_func mutils_warn_hook=mutils_default_proc;
static mutils_hexdump_func mutils_hexdump_hook=mutils_default_hexdump_proc;
static mutils_hexdump_func mutils_hexprint_hook=mutils_default_hexprint_proc;

static void mutils_error_va(char *fmt,va_list args)
{
    (*mutils_error_hook)(fmt,args);
}

static void mutils_info_va(char *fmt,va_list args)
{
    (*mutils_info_hook)(fmt,args);
}

static void mutils_debug_va(char *fmt,va_list args)
{
    (*mutils_debug_hook)(fmt,args);
}

static void mutils_warn_va(char *fmt,va_list args)
{
    (*mutils_warn_hook)(fmt,args);
}


void mutils_hexdump(unsigned char *bytes,int bytes_len)
{
    (*mutils_hexdump_hook)(bytes,bytes_len);
}

void mutils_hexprint(unsigned char *bytes,int bytes_len)
{
    (*mutils_hexprint_hook)(bytes,bytes_len);
}

/**
 * @brief   Turn debug on or off
 * @param d If 1, turn debug 1, if 0 turn debug off
 */
void mutils_set_debug(int d)
{
    if (d == 1)
    {
        s_mutils_debug=1;
    }
    else if (d == 0)
    {
        s_mutils_debug=0;
    }
}

/**
 * @brief Set application defined function for printing error messages
 */
void mutils_set_error_hook(mutils_error_func new_func)
{
    if (new_func)
    {
        mutils_error_hook=new_func;
    }
    else
    {
        mutils_error_hook=mutils_default_proc;
    }
}

/**
 * @brief Set application defined function for printing info messages
 */
void mutils_set_info_hook(mutils_error_func new_func)
{
    if (new_func)
    {
        mutils_info_hook=new_func;
    }
    else
    {
        mutils_info_hook=mutils_default_proc;
    }
}

void mutils_set_debug_hook(mutils_error_func new_func)
{
    if (new_func)
    {
        mutils_debug_hook=new_func;
    }
    else
    {
        mutils_debug_hook=mutils_default_proc;
    }
}
/**
 * @brief Set application defined function for printing warning messages
 */
void mutils_set_warn_hook(mutils_error_func new_func)
{
    if (new_func)
    {
        mutils_warn_hook=new_func;
    }
    else
    {
        mutils_warn_hook=mutils_default_proc;
    }
}

void mutils_set_hexdump_hook(mutils_hexdump_func new_func)
{
    if (new_func)
    {
        mutils_hexdump_hook=new_func;
    }
    else
    {
        mutils_hexdump_hook=mutils_default_hexdump_proc;
    }
}

void mutils_set_hexprint_hook(mutils_hexdump_func new_func)
{
    if (new_func)
    {
        mutils_hexprint_hook=new_func;
    }
    else
    {
        mutils_hexprint_hook=mutils_default_hexprint_proc;
    }
}



void mutils_reset_error_hook(void)
{
    mutils_set_error_hook(NULL);
}

void mutils_reset_info_hook(void)
{
    mutils_set_info_hook(NULL);
}

void mutils_reset_warn_hook(void)
{
    mutils_set_warn_hook(NULL);
}

/**
    @brief  print error message
    @param  fmt     The formatted parameters

    This function prints eror message to standard error if no application
    error printing is not hooked by calling mutils_set_error_hook()
 */
void mutils_error(char *fmt,...)
{
    va_list
        ap;

    va_start(ap,fmt);
    mutils_error_va(fmt,ap);
    va_end(ap);
}

void mutils_info(char *fmt,...)
{
    va_list
        ap;

    va_start(ap,fmt);
    mutils_info_va(fmt,ap);
    va_end(ap);
}

void mutils_warn(char *fmt,...)
{
    va_list
        ap;

    va_start(ap,fmt);
    mutils_warn_va(fmt,ap);
    va_end(ap);
}

void mutils_debug(char *fmt,...)
{
    va_list
        ap;

    va_start(ap,fmt);
    mutils_debug_va(fmt,ap);
    va_end(ap);
}
