/*
**  main for mailsend - a simple mail sender via SMTP protocol
**
**  Limitations and Comments:
**      I needed to send a alert mail from a bare-bone networked NT machine,
**      but could not find a simple mail sender to do this (not surprised!).
**      so I wrote this one!
**
**
**  Development History:
**      who                  when           why
**      muquit@muquit.com    Mar-23-2001    first cut
*/

#define __MAIN__    1

#include "mailsend.h"


/* exits after writing the usage */
static void usage(void)
{
    char
        **p;

    static char
        *options[]=
        {
"  -copyright            - show copyright information",
"  -4                    - Force to use IPv4 address of SMTP server",
"  -6                    - Force to use IPv6 address of SMTP server",
"  -smtp hostname/IP*    - Hostname/IP address of the SMTP server",
"  -port SMTP port       - SMTP port",
"  -d    domain          - domain name for SMTP HELO/EHLO",
"  -t    to,to..*        - email address/es of the recipient/s",
"  -cc   cc,cc..         - carbon copy address/es",
"  +cc                   - do not ask for Carbon Copy",
"  -ct   seconds         - Connect timeout. Default is 5 seconds",
"  -bc   bcc,bcc..       - blind carbon copy address/es",
"  +bc                   - do not ask for Blind carbon copy",
"  +D                    - don not add Date header",
"  -f    address*        - email address of the sender",
"  -sub  subject         - subject",
"  -lilst file           - a file containing a list of email addresses",
"  -log file             - write log messages to this file",
"  -separator character  - sepatorator used with -attach. Default is comma (,)",
"                          If used must be specified before -attach",                           
"  -attach file,mime_type,[i/a],[name],[content-id],[enc type] (i=inline,a=attachment)",
"                        - attach this file as attachment or inline",
"  -show_attach          - show attachment in verbose mode, default is no",
"  -cs   character set   - for text/plain attachments (default is us-ascii)",
"  -content-type type    - Message body content type. Default: multipart/mixed",
"  -H    \"header\"        - Add custom Header",
"  -M    \"one line msg\"  - attach this one line text message",
"  -enc type             - encoding type for one line message",
"                          valid types are base64, none",
"  -name \"Full Name\"     - add name in the From header",
"  -v                    - verbose mode",
"  -V                    - show version info",
"  -w                    - wait for a CR after sending the mail",
"  -rt  email_address    - add Reply-To header",
"  -rrr email_address    - request read receipts to this address",
"  -ssl                  - SMTP over SSL",
"  -starttls             - use STARTTLS if the server supports it",
"  -auth                 - try CRAM-MD5,LOGIN,PLAIN in that order",
"  -auth-cram-md5        - use AUTH CRAM-MD5 authentication",
"  -auth-plain           - use AUTH PLAIN authentication",
"  -auth-login           - use AUTH LOGIN authentication",
"  -user username        - username for ESMTP authentication",
"  -pass password        - password for ESMTP authentication",
"  -example              - show examples",
"  -ehlo                 - force EHLO",
"  -info                 - show SMTP server information",
"  -help                 - shows this help",
"  -q                    - quiet",
            (char *) NULL
        };
    (void) printf("\n");
    (void) printf(" Version: %.1024s\n\n",MAILSEND_VERSION);
    (void) printf(" Copyright: %.1024s\n\n",NO_SPAM_STATEMENT);
#ifdef HAVE_OPENSSL
    (void) fprintf(stdout," (Compiled with OpenSSL version: %s)\n",
                   SSLeay_version(SSLEAY_VERSION));
#else
    (void) fprintf(stdout," (Not compiled with OpenSSL)\n");
#endif /* HAVE_OPENSSL */

    (void) printf(" usage: mailsend [options]\n");
    (void) printf(" Where the options are:\n");

    for (p=options; *p != NULL; p++)
        (void) printf("%s\n",*p);

    (void) fprintf(stdout,"\n The options with * must be specified\n");
    (void) fprintf(stdout,
" Environment variables:\n"
"  SMTP_USER_PASS for plain text password (-pass)\n");


    exit(0);
}

static void show_examples(void)
{
    (void) fprintf(stdout," Example (Note: type without newline):\n");
(void) fprintf(stdout,
" Show server info\n"
" ================\n"
" mailsend -v -info -port 587 -smtp smtp.gmail.com\n"
" mailsend -v -info -ssl -port 465 -smtp smtp.gmail.com\n"
" mailsend -v -info -smtp smtp.example.com -ct 2\n\n");

(void) fprintf(stdout,
" STARTTLS + AUTHENTICATION\n"
" =========================\n");
(void) fprintf(stdout,
" mailsend -to user@gmail.com -from user@gmail.com\n"
" -starttls -port 587 -auth\n"
" -smtp smtp.gmail.com\n"
" -sub test +cc +bc -v\n"
" -user you -pass \"your_password\"\n");
(void) fprintf(stdout,
" Note: Password can be set by env var SMTP_USER_PASS instead of -pass\n\n");

(void) fprintf(stdout,
" SSL + AUTHENTICATION\n"
" ====================\n");
(void) fprintf(stdout,
" mailsend -to user@gmail.com -from user@gmail.com\n"
" -ssl -port 465 -auth\n"
" -smtp smtp.gmail.com\n"
" -sub test +cc +bc -v\n"
" -user you -pass \"your_password\"\n\n");
(void) fprintf(stdout,
" As -auth is specified, CRAM-MD5, LOGIN, PLAIN will be tried in that order.\n"
" Use -auth-cram-md5, -auth-plan, -auth-login for specific auth mechanism.\n\n"
" Note: Password can be set by env var SMTP_USER_PASS instead of -pass\n\n");

(void) fprintf(stdout,
" Attachments\n"
" ===========\n");
(void) fprintf(stdout,
" mailsend -f user@example.com -smtp 10.100.30.1\n"
"  -t user@example.com -sub test -attach \"file.txt,text/plain\"\n"
"  -attach \"/usr/file.gif,image/gif\" -attach \"file.jpeg,image/jpg\"\n\n");

(void) fprintf(stdout,
" The name of the attachment will be file.gif and file.jpeg.\n"
" If you want the name to be different, add the fourth argument with -attach.\n"
" Note: you MUST have to give the attachment type in this case\n\n");

(void) fprintf(stdout,
" mailsend -f user@example.com -smtp 10.100.30.1\n"
"  -t user@example.com -sub test -attach \"file.txt,text/plain\"\n"
"  -attach \"/usr/file.gif,image/gif,a,bar.gif\" -attach \"file.jpeg,image/jpg\"\n\n");


(void) fprintf(stdout,
" mailsend -f user@example.com -smtp 192.168.0.2\n"
"  -t user@example.com -sub test +cc +bc\n"
"  -attach \"c:\\file.gif,image/gif\" -M \"Sending a GIF file\"\n\n");

(void) fprintf(stdout,
" mailsend -f user@example.com -smtp 192.168.0.2\n"
"  -t user@example.com -sub test +cc +bc -cs \"ISO-8859-1\"\n"
"  -attach \"file2.txt,text/plain\"\n\n");

(void) fprintf(stdout,
" Inline Attachment\n"
" =================\n");
    (void) fprintf(stdout,
" mailsend -f user@example.com -d example.com -smtp 10.100.30.1\n"
"  -t user@example.com -sub test -attach \"nf.jpg,image/jpeg,i\"\n"
"  -M \"body line1: content disposition is inline\"\n"
"  -M \"body line2: this is line2 of the body\"\n\n");
}

int main(int argc,char **argv)
{
    char
        *x,
        *y,
        buf[BUFSIZ],
        encrypted_pass[128], /* 80 bytes actually */
        *cipher=NULL,
        *option;

    int
        smtp_info=0,
        is_mime=0,
        add_dateh=1,
        port=(-1),
        rc = (-1),
        no_cc=1,
        no_bcc=1,
        i;

    char
        *address_file=NULL,
        *helo_domain=NULL,
        *smtp_server=NULL,
        *attach_file=NULL,
        *msg_body_file=NULL,  /* not used anymore */
        *the_msg=NULL,
        *custom_header=NULL,
        *to=NULL,
        *save_to=NULL,
        *save_cc=NULL,
        *save_bcc=NULL,
        *from=NULL,
        *sub=NULL,
        *cc=NULL,
        *bcc=NULL,
        *rt=NULL,
        *rrr=NULL;
 
    g_verbose=0;
    g_connect_timeout = DEFAULT_CONNECT_TIMEOUT; /* 5 secs */
    g_quiet=0;
    g_wait_for_cr=0;
    g_do_auth=0;
    g_esmtp=0;
    g_auth_plain=0;
    g_auth_cram_md5=0;
    g_auth_login=0;
    g_do_ssl=0;
    g_do_starttls=0;
    g_log_fp = NULL;
    g_show_attachment_in_log = 0;
    g_use_protocol = MSOCK_USE_AUTO; /* detect IPv4 or IPv6 */
    g_encoding_type = ENCODE_NONE;

    memset(g_log_file,0,sizeof(g_log_file));
    memset(g_username,0,sizeof(g_username));
    memset(g_userpass,0,sizeof(g_userpass));
    memset(encrypted_pass, 0, sizeof(encrypted_pass));
    memset(g_from_name,0,sizeof(g_from_name));
	memset(g_content_type,0,sizeof(g_content_type));
    memset(g_attach_sep, 0, sizeof(g_attach_sep));
    (void) strcpy(g_attach_sep,",");
	
    (void) strcpy(g_charset,"us-ascii");

    for  (i=1; i < argc; i++)
    {
        option=argv[i];
        switch (*(option+1))
        {

            case 'a':
            {
                if (strncmp("attach",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing file to attach");
                            return (1);
                        }
                        attach_file=argv[i];
                        add_attachment_to_list(attach_file);
                    }
                }
                else if (strncmp("auth-plain",option+1,
                            strlen("auth-plain"))==0)
                {
                    if (*option == '-')
                    {
                        g_auth_plain=1;
                    }
                }
                else if (strncmp("auth-cram-md5",option+1,
                            strlen("auth-cram-md5"))==0)
                {
                    if (*option == '-')
                    {
                        g_auth_cram_md5=1;
                    }
                }
                else if (strncmp("auth-login",option+1,
                            strlen("auth-login"))==0)
                {
                    if (*option == '-')
                    {
                        g_auth_login=1;
                    }
                }
                else if (strncmp("auth",option+1,
                            strlen("auth"))==0)
                {
                    if (*option == '-')
                    {
                        g_auth_plain=1;
                        g_auth_login=1;
                        g_auth_cram_md5=1;
                        g_auth_login=1;
                        g_do_auth=1;
                    }
                }

                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }
                break;
            }

            case 'b':
            {
                if (strncmp("bcc",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing BCc address/es");
                            return (1);
                        }
                        bcc=argv[i];
                        save_bcc=mutilsStrdup(bcc);

                        /* collapse all spaces to a comma */
                        mutilsSpacesToChar(bcc,',');
                        addAddressToList(bcc,"Bcc");
                    }
                    else if (*option == '+')
                    {
                        no_bcc=1;
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }
                break;
            }

            case '4':
            {
                if (strncmp("4", option + 1,1) == 0)
                {
                    if (*option == '-')
                    {
                        g_use_protocol = MSOCK_USE_IPV4;
                    }
                }
                break;
            }

            case '6':
            {
                if (strncmp("6", option + 1,1) == 0)
                {
                    if (*option == '-')
                    {
                        g_use_protocol = MSOCK_USE_IPV6;
                    }
                }

                break;
            }



            case 'c':
            {
                if (strncmp("cc",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing Cc address/es");
                            return (1);
                        }
                        cc=argv[i];
                        save_cc=mutilsStrdup(cc);

                        /* collapse all spaces to a comma */
                        mutilsSpacesToChar(cc,',');
                        addAddressToList(cc,"Cc");
                    }
                    else if (*option == '+')
                    {
                        no_cc=1;
                    }
                }
                else if (strncmp("cs",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing character set");
                            return (1);
                        }
                        mutilsSafeStrcpy(g_charset,argv[i],sizeof(g_charset)-1);
                    }

                }
                else if (strncmp("ct", option + 1, 2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing connect timeout with -ct");
                            return (1);
                        }
                        g_connect_timeout = atoi(argv[i]);
                    }
                } else if (strncmp("content-type", option+1, 12)==0) 
				{
					 if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing content-type set");
                            return (1);
                        }
                        mutilsSafeStrcpy(g_content_type,argv[i],sizeof(g_content_type)-1);
                    }
				}
                else if (strncmp("copyright", option + 1, 4) == 0)
                {
                    print_copyright();
                    exit(0);
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }
                break;
            }

            case 'D':
            {
                if (strncmp("D",option+1,1) == 0)
                {
                    if (*option == '+')
                    {
                        add_dateh=0;
                    }
                }
                break;
            }


            case 'd':
            {
                if (strncmp("domain",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing domain name");
                            return (1);
                        }
                        helo_domain=argv[i];
                    }
                }
                break;
            }

            case 'e':
            {
                if (*option == '-')
                {
                    if (strncmp("example",option+1,2) == 0)
                    {
                        show_examples();
                        return(1);
                    }
                    if (strncmp("ehlo",option+1,4) == 0)
                    {
                        g_esmtp=1;
                    }

                    if (strncmp("enc",option+1,3) == 0)
                    {
                        if (*option == '-')
                        {
                            i++;
                            if (i == argc)
                            {
                                errorMsg("Missing encoding type");
                                return (1);
                            }
                            g_encoding_type = get_encoding_type(argv[i]);
                        }
                    }
                }
                break;
            }

            case 'f':
            {
                if (strncmp("from",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing From address/es");
                            return (1);
                        }
                        from=argv[i];
                    }
                }
                break;
            }

            case 'h':
            {
                if (strncmp("help",option+1,1) == 0)
                {
                    usage();
                }
                /* won't be here */
                break;
            }

            case 'i':
            {
                if (strncmp("info",option+1,2) == 0)
                {
                    smtp_info=1;
                }
                break;
            }

            case 'l':
            {
                if (strncmp("list_address",option+1,3) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing address list file"); 
                            return (1);
                        }
                        address_file=argv[i];
                    }
                }

                if (strncmp("log",option+1,3) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing address log file"); 
                            return (1);
                        }
                        g_log_fp = fopen(argv[i], "a");
                        if (g_log_fp == NULL)
                        {
                            errorMsg("Could not open log file %s for writing (%s)",
                                    argv[i],
                                    strerror(errno));
                            return(1);
                        }
                        /*
                        ** tell msock lib to write error message to log file
                        */
                        msock_set_logfp(g_log_fp);
                        mutilsSafeStrcpy(g_log_file,argv[i],sizeof(g_log_file)-1);
                    }
                }
                break;
            }

            case 'p':
            {
                if (strncmp("port",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing SMTP Port with -port");
                            return (1);
                        }
                        port=atoi(argv[i]);
                    }
                }
                else if (strncmp("pass",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing password with -pass");
                            return (1);
                        }
                        (void) snprintf(g_userpass,sizeof(g_userpass)-1,
                                        "%s",argv[i]);
                    }

                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }
                break;
            }

            case 'H':
            {
                if (strncmp("Header",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing custom header");
                            return (1);
                        }
                        custom_header=argv[i];
                        add_customer_header_to_list(custom_header);
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }

                break;
            }


            case 'M':
            {
                if (strncmp("Message",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing text message");
                            return (1);
                        }
                        the_msg=argv[i];
                        add_one_line_to_list(the_msg);
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }

                break;
            }

            case 'n':
            {
                if (strncmp("name",option+1,3) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing Name with -n");
                            return (1);
                        }
                        (void) snprintf(g_from_name,sizeof(g_from_name)-1,
                                        "%s",argv[i]);
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }

                break;
            }

            case 's':
            {
                if (strncmp("smtp",option+1,3) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing smtp server");
                            return (1);
                        }
                        smtp_server=argv[i];
                    }
                }
                else if (strncmp("subject",option+1,3) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing subject with -sub");
                            return (1);
                        }
                        sub=argv[i];
                    }
                }
                else if (strncmp("ssl", option + 1, 3) == 0)
                {
#ifdef HAVE_OPENSSL
                    g_do_ssl=1;
#else
                    (void) fprintf(stderr,"Error: '-ssl' not available, not compiled with OpenSSL\n");
                    return(1);
#endif /* HAVE_OPENSSL */

                }
                else if (strncmp("starttls",option+1,3) == 0)
                {
#ifdef HAVE_OPENSSL
                    g_do_starttls=1;
#else
                    (void) fprintf(stderr,"Error: '-starttls' not available, not compiled with OpenSSL\n");
                    return(1);
                    
#endif /* HAVE_OPENSSL */
                }
                else if (strncmp("show_attach", option + 1, 9) == 0)
                {
                    g_show_attachment_in_log = 1;
                }
                else if (strncmp("separator", option + 1, 4) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing separator");
                            return (1);
                        }
                        (void) snprintf(g_attach_sep,sizeof(g_attach_sep)-1,"%s",argv[i]);
                        if (strlen(g_attach_sep) != 1)
                        {
                            errorMsg("Invalid separator character specified");
                            return(1);
                        }
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }

                break;
            }

            case 'u':
            {
                if (strncmp("user",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            errorMsg("Missing smtp server");
                            return (1);
                        }
                        (void) snprintf(g_username,sizeof(g_username)-1,
                                        "%s",argv[i]);
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }

                break;
            }



            case 'v':
            {
                if (strncmp("verbose",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        g_verbose=1;
                        msock_set_debug(g_verbose);
                    }
                }
                break;
            }

            case 'q':
            {
                if (strncmp("quiet",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        g_quiet=1;
                    }
                }
                break;
            }
            

            case 'V':
            {
                (void) fprintf(stderr,"mailsend Version: %.1024s\n",MAILSEND_VERSION);
#ifdef HAVE_OPENSSL
                (void) fprintf(stderr,"Compiled with OpenSSL: %s\n",
                               SSLeay_version(SSLEAY_VERSION));
#else
                (void) fprintf(stderr,"Not Compiled OpenSSL, some auth methods will be unavailable\n");
#endif /* ! HAVE_OPENSSL */
                return(0);
                break;
            }

           case 't':
           {
                if (strncmp("to",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            (void) fprintf(stderr,"Error: missing to addresses\n");
                            return (1);
                        }
                        to=argv[i];
                        save_to=mutilsStrdup(to);
                        if (save_to == NULL)
                        {
                            errorMsg("memory allocation problem for -to");
                            return(-1);
                        }
                        save_to=fix_to(save_to);
                        to=fix_to(to);
                        /* collapse all spaces to a comma */
                        mutilsSpacesToChar(to,',');

                        /* add addresses to a singly linked list */
                        addAddressToList(to,"To");
                        /* Note: to is modifed now! */
                    }
                }
                break;
            }

            case 'r':
            {
                if (strncmp("rrr",option+1,3) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            (void) fprintf(stderr,"Error: missing to addresses for -rrr\n");
                            return (1);
                        }
                        rrr=mutilsStrdup(argv[i]);
                        if (rrr == NULL)
                        {
                            errorMsg("memory allocation problem for -rrr");
                            return(1);
                        }
                    }
                }
                else if (strncmp("rt",option+1,2) == 0)
                {
                    if (*option == '-')
                    {
                        i++;
                        if (i == argc)
                        {
                            (void) fprintf(stderr,"Error: missing to addresses for -rt\n");
                            return (1);
                        }
                        rt=mutilsStrdup(argv[i]);
                        if (rt == NULL)
                        {
                            errorMsg("memory allocation problem for -rt");
                            return(1);
                        }
                    }
                }
                else
                {
                    errorMsg("Unknown flag: %s\n",option);
                    return(1);
                }

                break;
            }

            case 'w':
            {
                if (strncmp("wait",option+1,1) == 0)
                {
                    if (*option == '-')
                    {
                        g_wait_for_cr=1;
                    }
                }

                break;
            }

            default:
            {
                (void) fprintf(stderr,"Error: Unrecognized option: %s\n",
                               option);
                return (1);
            }


        }
    }


    x=getenv("SMTP_USER_PASS");
    y=NULL;
    if (x && y)
    {
        (void) fprintf(stderr,"SMTP_USER_PASS and SMTP_USER_PASS_ENC can not be set. Exiting..\n");
        return(1);
    }
    if (x)
    {
        if (*g_userpass == '\0')
        {
            (void) snprintf(g_userpass,sizeof(g_userpass)-1,"%s",x);
        }
    }

#ifdef HAVE_OPENSSL
    if (g_do_ssl && g_do_starttls)
    {
        (void) fprintf(stderr,"Options -ssl and -starttls are mutually exclusive\n");
        exit(1);
    }
#endif /* HAVE_OPENSSL */

    initialize_openssl(cipher);

    if (port == -1)
        port=MAILSEND_SMTP_PORT;
    if (smtp_info)
    {
        if (smtp_server == NULL)
            smtp_server="localhost";
    }
    if (smtp_info && smtp_server)
    {
        if (helo_domain == NULL)
            helo_domain="localhost";
        rc = show_smtp_info(smtp_server,port,helo_domain);
        if (rc < 0)
            return (1);
        return(0);
    }


    print_attachemtn_list();

    /*
    ** attaching a file or a one line message will make the mail a 
    ** MIME mail
    */
    if (attach_file || the_msg || msg_body_file)
    {
        is_mime=1;
    }

    if (smtp_server == NULL)
    {
        memset(buf,0,sizeof(buf));
        x=askFor(buf,sizeof(buf)-1,"SMTP server address/IP: ",EMPTY_NOT_OK);
        if (x)
            smtp_server=xStrdup(x);
    }

    if (helo_domain == NULL)
    {
        /*
        memset(buf,0,sizeof(buf));
        x=askFor(buf,sizeof(buf)-1,"Domain: ",EMPTY_NOT_OK);
        if (x)
            helo_domain=xStrdup(x);
        */
        /* use localhost */
        helo_domain=xStrdup("localhost");
    }

    if (from == NULL)
    {
        memset(buf,0,sizeof(buf));
        x=askFor(buf,sizeof(buf)-1,"From: ",EMPTY_NOT_OK);
        if (x)
            from=xStrdup(x);
    }

    /* if address file specified, add the addresses to the list as well */
    if (address_file != NULL)
    {
        addAddressesFromFileToList(address_file);
        printAddressList();
    }

   /*
    ** The To address must be speicifed, even if the file with the list of
    ** addresses is specified. The specified To will be shown in the 
    ** envelope. None of the To, Cc and Bcc from the address list file will
    ** be shown anywhre.. that's how I like it. I hate long To, Cc or Bcc.
    ** muquit@muquit.com, Thu Mar 29 11:56:45 EST 2001 
    */

    if (save_to == NULL)
    {
        /* don't ask for To add addresses are specified with -l file */
        if (getAddressList() == NULL)
        {
            memset(buf,0,sizeof(buf));
            x=askFor(buf,sizeof(buf)-1,"To: ",EMPTY_NOT_OK);
            if (x)
            {
                save_to=xStrdup(x);
                addAddressToList(x,"To");
            }
        }
    }

    /*
    ** if msg file specified, dont ask for unneeded things, as it could
    ** be used from other programs, and it will wait for input
    ** muquit@muquit.com Tue Apr 10 18:02:12 EST 2001 
    */

#ifdef WINNT
    if (attach_file == NULL && isInConsole(_fileno(stdin)))
#else
    if (attach_file == NULL && isatty(fileno(stdin)))
#endif /* WINNT */
    {
        if (save_cc == NULL && !no_cc)
        {
            memset(buf,0,sizeof(buf));
            x=askFor(buf,sizeof(buf)-1,"Carbon copy: ",EMPTY_OK);
            if (x)
            {
                save_cc=xStrdup(x);
                addAddressToList(x,"Cc");
            }
        }

        if (save_bcc == NULL && ! no_bcc)
        {
            memset(buf,0,sizeof(buf));
            x=askFor(buf,sizeof(buf)-1,"Blind Carbon copy: ",EMPTY_OK);
            if (x)
            {
                save_bcc=xStrdup(x);
                addAddressToList(x,"BCc");
            }
        }

        if (sub == NULL)
        {
            memset(buf,0,sizeof(buf));
            x=askFor(buf,sizeof(buf)-1,"Subject: ",EMPTY_OK);
            if (x)
                sub=xStrdup(x);
        }

        if (g_do_ssl || g_do_starttls)
        {
            if (*g_username == '\0')
            {
                memset(buf,0,sizeof(buf));
                x=askFor(buf,sizeof(buf)-1,"Auth user: ",EMPTY_NOT_OK);
                if (x)
                {
                    (void) snprintf(g_username,sizeof(g_username)-1,"%s",x);
                }
            }

            if (*g_userpass == '\0')
            {
                (void) fprintf(stderr,"\nPlease specify auth password with -pass\n");
                (void) fprintf(stderr,"Or by environment variable SMTP_USER_PASS\n");
                return(1);
            }
        }
    }

    rc=validateMusts(from,save_to,smtp_server,helo_domain);
    if (rc != 0)
        return (1); /* exit */

#ifdef UNIX
    signal(SIGPIPE,SIG_IGN);
#endif /* UNIX */

#if 0
    {
        MutilsTime
            mt;
        char
            timebuf[64];
        mutils_time_now(&mt);
        mutils_time_fmt(&mt, timebuf, sizeof(timebuf));
        (void) fprintf(stderr,"timebuf: %s\n",timebuf);
        return 1;
    }
#endif /* 0 */

    write_log("mailsend v%s\n",MAILSEND_VERSION);
    rc=send_the_mail(from,save_to,save_cc,save_bcc,sub,smtp_server,port,
                helo_domain,attach_file,msg_body_file,the_msg,is_mime,rrr,rt,add_dateh);

    if (rc == 0)
    {
        if (isInteractive())
        {
            if (!g_quiet)
            {
                (void) printf("Mail sent successfully\n");
                (void) fflush(stdout);
            }
        }
        if (!g_quiet)
        {
            write_log("Mail sent successfully\n");
        }
    }
    else
    {
        if (isInteractive())
        {
            (void) printf("Could not send mail\n");
        }
        write_log("Could not send mail\n");
    }

    if (isInteractive())
    {
        if (g_wait_for_cr)
        {
            printf("\nPress Enter to Exit: ");
            x=fgets(buf,sizeof(buf)-1,stdin);
        }
    }

    if (rc != 0)
    {
        rc = 1;
    }
    return (rc);
}
