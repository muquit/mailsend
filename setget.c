/*
**  setget routines for mailsend
**
**  Limitations and Comments:
**      vars are static in this module.
**
**
**  Development History:
**      who                  when           why
**      muquit@muquit.com    Mar-27-2001    first cut
*/

#include "mailsend.h"

static Sll
    *one_line_head = NULL,
    *custom_headers_head = NULL,
    *attachment_head=NULL,
    *oneline_attachment_head = NULL,
    *msg_body_attachment_head = NULL,
    *embed_image_attachment_head = NULL,
    *server_caps=NULL,
    *addr_head=NULL;

static Address
    *address=NULL;

void print_server_caps(void)
{
    Sll
        *l;
    for (l=server_caps; l; l=l->next)
    {
        (void) fprintf(stdout,"%s\n",(char *) l->data);
        (void) fflush(stdout);
    }
}

void print_one_lines(void)
{
    Sll
        *l;
    for (l = one_line_head; l; l = l->next)
    {
        (void) fprintf(stdout,"LINE: '%s'\n",(char *) l->data);
        (void) fflush(stdout);
    }
}


void printAddressList2(Sll *list)
{
/*
    Sll
        *l;

    Address
        *addr=NULL;

    for (l=list; l; l=l->next)
    {
        addr=(Address *) l->data;
    }
*/
}

void print_oneline_attachment_list(void)
{
    Sll
        *l;

    Attachment
        *a;

    for (l=oneline_attachment_head; l; l=l->next)
    {
        a=(Attachment *) l->data;
        (void) fprintf(stderr,"Message: %s\n",a->oneline_msg);
        (void) fprintf(stderr,"Mime type: %s\n",a->mime_type);
        (void) fprintf(stderr,"Disposition: %s\n",a->content_disposition);
        (void) fprintf(stderr,"Encoding type: %s\n",a->content_transfer_encoding);
        (void) fprintf(stderr,"\n");
    }
}


void print_attachment_list(void)
{
    Sll
        *l;

    Attachment
        *a;

    for (l=attachment_head; l; l=l->next)
    {
        a=(Attachment *) l->data;
        (void) fprintf(stderr,"File path: %s\n",a->file_path);
        (void) fprintf(stderr,"File name: %s\n",a->file_name);
        if (a->attachment_name)
            (void) fprintf(stderr,"Attachment name: %s\n",a->attachment_name);
        (void) fprintf(stderr,"Mime type: %s\n",a->mime_type);
        (void) fprintf(stderr,"Disposition: %s\n",a->content_disposition);
        if (a->content_id)
            (void) fprintf(stderr,"Content-ID: %s\n",a->content_id);
        (void) fprintf(stderr,"Encoding type: %s\n",a->content_transfer_encoding);
        (void) fprintf(stderr,"\n");
    }
}


void printAddressList(void)
{
    Sll
        *l;

    Address
        *addr=NULL;

    if (g_verbose)
    {
        for (l=addr_head; l; l=l->next)
        {
            addr=(Address *) l->data;
            if (addr)
            {
                (void) fprintf(stdout,"%s: %s\n",
                               addr->label,
                               addr->address);
            }
        }
    }
}

/* return pointer to  capability if found, NULL otherwise */
char *check_server_cap(char *what)
{
    Sll
        *l;
    if (what == NULL || *what == '\0')
        return(NULL);

    for (l=server_caps; l; l=l->next)
    {
        if (mutilsStristr((char *) l->data,what))
        {
            return((char *) l->data);
        }
    }
    return(NULL);
}

/* add server capabilities to our list */
int add_server_cap_to_list(char *cap)
{
    Sll 
        *na=NULL;

    char
        *c;

    if (cap == NULL || *cap == '\0')
        return(-1);


    c=strdup(cap);
    if (c == NULL)
        return(-1);

    na=allocateNode((void *) c);
    CHECK_MALLOC(na);
    appendNode(&server_caps,&na);

    return(0);
}

/*
** Add each line entered with -H to list of lines
** they represent custom headers in the mail.
** For example X-Priority etc. can be added that way
*/
int add_customer_header_to_list(char *line)
{
    Sll
        *nl = NULL;

    char
        *l;

    if (line == NULL || *line == '\0')
    {
        return (-1);
    }
    l = strdup(line);
    CHECK_MALLOC(l);
    nl = allocateNode((void *) l);
    CHECK_MALLOC(nl);
    appendNode(&custom_headers_head, &nl);

    return(0);
}

/*
** Add each line entered with -M to a list of lines
** return 0 on success, -1 otherwise
*/
int add_one_line_to_list(char *line)
{
    Sll
        *nl = NULL;

    char
        *l;

    if (line == NULL || *line == '\0')
    {
        return (-1);
    }
    l = strdup(line);
    CHECK_MALLOC(l);
    nl = allocateNode((void *) l);
    CHECK_MALLOC(nl);
    appendNode(&one_line_head, &nl);

    return(0);
}


/**
 * add the file to attachment_list
 */
int add_attachment_to_list(char *file_path_mime)
{
    int
        rc,
        separator,
        ntokens;

    Sll
        *na=NULL;

    char
        **tokens=NULL,
        *file_path=NULL,
        *file_name=NULL,
        *mime_type,
        *content_disposition="attachment";

    Attachment
        *a=NULL;

    if (file_path_mime == NULL || *file_path_mime == '\0')
        return(-1);

    /* Tokenize the string "file,mime_type,something" */
    separator = *g_attach_sep;
    showVerbose("Separator: %c\n",separator);
    tokens=mutilsTokenize(file_path_mime,separator,&ntokens);
    if (tokens == NULL)
    {
        errorMsg("Could not parse attachment string: \"%s\"",file_path_mime);
        exit(1);
    }

    showVerbose("ntokens: %d\n",ntokens);

    /* get the file name out */
    file_path=tokens[0];
    if ((file_name=strrchr(file_path,'/')) || 
        (file_name=strrchr(file_path,'\\')))
    {
        file_name++;
    }
    else
    {
        file_name=file_path;
    }

    a = allocate_attachment();
    a->file_path=xStrdup(file_path);
    a->file_name=xStrdup(file_name);

    switch (ntokens)
    {
        case 1: /* Only File_path/name given */
        {

            /*
            mime_type="application/octet-stream";
            a->mime_type=xStrdup(mime_type);
            a->content_disposition=xStrdup("attachment");
            */
            break;
        }

        case 2: /* filepath/name, mime_type given */
        {
            mime_type=tokens[1];
            a->mime_type=xStrdup(mime_type);
            break;
        }

        case 3: /* filepath/name, mime_type, disposition given */
        case 4: /* filepath/name, mime_type, disposition, attchment name given */
        case 5: /* content-id */
        case 6: /* encoding type */
        {
            mime_type=tokens[1];
            a->mime_type=xStrdup(mime_type);
            content_disposition=tokens[2];
            if (*content_disposition == 'a')
            {
                a->content_disposition=xStrdup("attachment");
            }
            else if (*content_disposition == 'i')
            {
                a->content_disposition=xStrdup("inline");
            }
            else
            {
                a->content_disposition=xStrdup("attachment");
            }

            if (ntokens == 4)
            {
                a->attachment_name = xStrdup(tokens[3]);
            }
            if (ntokens == 5)
            {
                a->attachment_name = xStrdup(tokens[3]);
                if (strncmp(tokens[4],"none",4) != 0)
                    a->content_id = xStrdup(tokens[4]);
            }
            if (ntokens == 6)
            {
                a->attachment_name = xStrdup(tokens[3]);
                if (strncmp(tokens[4],"none",4) != 0)
                    a->content_id = xStrdup(tokens[4]);
                a->content_transfer_encoding = xStrdup(tokens[5]);
            }
            break;
        }
        
        default:
        {
            errorMsg("Invalid string specified with -a \"%s\"",file_path_mime);
            exit(1);
            break;
        }
    }

    rc = mutils_file_is_binary(a->file_path);
    if (rc == -1)
    {
        errorMsg("Could not determine file type of %s",a->file_path);
    }
    if (rc == MUTILS_TRUE)
    {
        mime_type = get_mime_type(a->file_path);
        if (*g_mime_type != '\0')
        {
            if ((mutilsStrcasecmp(g_mime_type,"text/html") == 0) ||
                (mutilsStrcasecmp(g_mime_type,"text/plain") == 0))
            {
                a->charset = xStrdup("none");
                a->mime_type = xStrdup(mime_type);
                a->content_transfer_encoding = xStrdup("base64");
            }
        }
    }


    if (a->mime_type == NULL)
    {
        if (*g_mime_type != '\0')
        {
            a->mime_type = xStrdup(g_mime_type);
        }
        else
        {
            a->mime_type = xStrdup(get_mime_type(a->file_path));
        }
    }
    if (a->content_transfer_encoding == NULL)
    {
        if (*g_content_transfer_encoding == '\0')
        {
            a->content_transfer_encoding = xStrdup("base64");
        }
    }

    if (a->content_id == NULL)
    {
        if (*g_content_id != '\0')
        {
            a->content_id = xStrdup(g_content_id);
        }
    }

    na=allocateNode((void *) a);
    CHECK_MALLOC(na);

    appendNode(&attachment_head,&na);

    /*print_attachemtn_list();*/
    return(0);
}

int add_embed_image_to_attachment_list(const char *image_file)
{
    Sll
        *na=NULL;

    Attachment
        *a=NULL;

    if (image_file == NULL || *image_file == '\0')
    {
        return(-1);
    }
    a=allocate_attachment(); /* defaults will be set */

    a->file_path = xStrdup(image_file);
    if (a->charset)
    {
        (void) free((char *) a->charset);
        a->charset = xStrdup("none");
    }

    if (a->mime_type == NULL)
    {
        if (*g_mime_type != '\0')
        {
            a->mime_type = xStrdup(g_mime_type);
        }
        else
        {
            a->mime_type = xStrdup(get_mime_type(a->file_path));
        }
    }

    if (a->content_transfer_encoding == NULL)
    {
        if (*g_content_transfer_encoding == '\0')
        {
            a->content_transfer_encoding = xStrdup("base64");
        }
    }


    na=allocateNode((void *) a);
    CHECK_MALLOC(na);
    appendNode(&embed_image_attachment_head,&na);
    return(0);

}

int add_msg_body_to_attachment_list(const char *msg_body_file)
{
    Sll
        *na=NULL;

    Attachment
        *a=NULL;

    if (msg_body_file == NULL || *msg_body_file == '\0')
    {
        return(-1);
    }
    a=allocate_attachment(); /* defaults will be set */

    a->file_path = xStrdup(msg_body_file);

    na=allocateNode((void *) a);
    CHECK_MALLOC(na);
    appendNode(&msg_body_attachment_head,&na);
    return(0);
}


/*
** create a list of Attachment for one line messages
** it uses -mime-type and -enc-type, so these two flags
** must specified first before -M
*/
int add_oneline_to_attachment_list(char *oneline_msg)
{
    Sll
        *na=NULL;

    Attachment
        *a=NULL;

    if (oneline_msg == NULL || *oneline_msg == '\0')
    {
        return(-1);
    }
    a=allocate_attachment(); /* defaults will be set */

    a->oneline_msg=xStrdup(oneline_msg);
    a->content_disposition=xStrdup("inline");
    if (*g_mime_type == '\0')
    {
        a->mime_type = xStrdup("text/plain");
    }

    na=allocateNode((void *) a);
    CHECK_MALLOC(na);

    appendNode(&oneline_attachment_head,&na);
    return(0);
}



int addAddressToList(char *a,char *label)
{
    char
        *p;

    Sll
        *new=(Sll *) NULL;

    if (a == NULL || *a == '\0')
        return(-1);

    if (label == NULL || *label == '\0')
        label="To";

    /* if comma separated, tokenize them */
    p=mutilsStrtok(a,",");
    if (p != NULL)
    {
        address=newAddress();
        if (address == NULL)
        {
            errorMsg("addAddressToList: malloc problem for newAddress()");
            return (-1);
        }

        address->label=xStrdup(label);
        address->address=xStrdup(p);

        new=allocateNode((void *) address);
        if (new == (Sll *) NULL)
        {
            errorMsg("addAddressToList: malloc problem with allocateNode()");
            (void) free ((char *) address);
            return (-1);
        }
        appendNode(&addr_head,&new);
    }
    while ((p=mutilsStrtok((char *) NULL,",")) != NULL)
    {
        if (p != NULL)
        {
            address=newAddress();
            if (address == NULL)
            {
                errorMsg("addAddressToList: x malloc problem for newAddress()");
                return (-1);
            }
            address->label=xStrdup(label);
            address->address=xStrdup(p);
            new=allocateNode((void *) address);
            if (new == (Sll *) NULL)
            {
                errorMsg("addAddressToList:malloc problem with allocateNode()");
                (void) free ((char *) address);
                return (-1);
            }
            appendNode(&addr_head,&new);
        }

    }

    return (1);
}



/*
** add the address list from file to the "head" linked list.
** return 0 on success, -1 on error
*/
int addAddressesFromFileToList(char *address_file)
{
    char
        *label=NULL,
        *p=NULL,
        buf[BUFSIZ];

    Sll
        *list=(Sll *) NULL;

    Address
        *addr=(Address *) NULL;

    FILE
        *fp=(FILE *) NULL;

    /* open the file */
    fp=fopen(address_file,"r");
    if (fp == (FILE *) NULL)
    {
        errorMsg("Could not open address list file: %s",address_file);
        return (-1);
    }
    while (fgets(buf,sizeof(buf)-1,fp) && !feof(fp))
    {
        p=NULL;
       
        /* remove all white spaces */
        mutilsRmallws(buf);

        if (*buf == '\0')
            continue;
        /* If a line starts with # or ; consider it's a comment */
        if (*buf == '#' || *buf == ';')
            continue;

        showVerbose("Address> %s\n",buf);
        if (mutilsStrncasecmp(buf,"To:",3) == 0)
        {
            label="To";
            if (strlen(buf) > 3)
                p=buf+3;
            else
                p=(char *) NULL;
        }
        else if (mutilsStrncasecmp(buf,"Cc:",3) == 0)
        {
            label="Cc";
            if (strlen(buf) > 3)
                p=buf+3;
            else
                p=(char *) NULL;
        }
        else if (mutilsStrncasecmp(buf,"BCc:",4) == 0)
        {
            label="BCc";
            if (strlen(buf) > 4)
                p=buf+4;
            else
                p=(char *) NULL;
        }
        else
        {
            /* just an email address */
            /* TODO: allow comma separared may be */
            label="To";
            p=buf;
        }

        if (p == (char *) NULL) /* won't be */
            p=buf;

        if (p == (char *) NULL)
            continue;

        addr=newAddress();
        if (addr == NULL)
        {
            errorMsg("addAddressToList: malloc problem for newAddress()");
            return (-1);
        }
        /* fill with data */
        showVerbose("Label: %s\n",label);
        showVerbose("Address: %s\n",p);

        addr->label=xStrdup(label);
        addr->address=xStrdup(p);
        list=allocateNode((void *) addr);
        if (list == (Sll *) NULL)
        {
            errorMsg("addAddressToList: malloc problem with allocateNode()");
            (void) free ((char *) addr);
            return (-1);
        }
        appendNode(&addr_head,&list);
    }
    
    if (fp != (FILE *) NULL)
        (void) fclose(fp);

    return (0);
}


Sll *getAddressList(void)
{
    return (addr_head);
}

Sll *get_one_line_list(void)
{
    return (one_line_head);
}

Sll *get_custom_header_list(void)
{
    return (custom_headers_head);
}

Sll *get_attachment_list(void)
{
    return(attachment_head);
}
Sll *get_oneline_attachment_list(void)
{
    return(oneline_attachment_head);
}

Sll *get_server_cap_list(void)
{
    return(server_caps);
}

Sll *get_msg_body_attachment_list(void)
{
    return(msg_body_attachment_head);
}

Sll *get_embed_image_attachment_list(void)
{
    return (embed_image_attachment_head);
}

Attachment *allocate_attachment(void)
{
    Attachment
        *a;
    a = (Attachment *) malloc(sizeof(Attachment));
    CHECK_MALLOC(a);
    memset(a,0,sizeof(Attachment));

    a->charset = xStrdup(g_charset); /* default is "utf-8" */
    if (*g_mime_type != '\0')
    {
        a->mime_type = xStrdup(g_mime_type);
    }

    if (*g_content_transfer_encoding != '\0')
        a->content_transfer_encoding = xStrdup(g_content_transfer_encoding);

    a->attach_separator = *g_attach_sep;
    a->content_disposition = xStrdup(g_content_disposition);
    if (*g_attach_name != '\0')
    {
        a->attachment_name = xStrdup(g_attach_name);
    }

    return(a);
}

/* just a debug routine */

