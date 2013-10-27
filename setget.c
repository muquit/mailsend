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

void print_attachemtn_list()
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
        ntokens;

    Sll
        *na=NULL;

    char
        **tokens=NULL,
        *file_path=NULL,
        *file_name=NULL,
        *mime_type=NULL,
        *content_disposition="attachment";

    Attachment
        *a=NULL;

    if (file_path_mime == NULL || *file_path_mime == '\0')
        return(-1);

    /* Tokenize the string "file,mime_type,something" */
    tokens=mutilsTokenize(file_path_mime,',',&ntokens);
    if (tokens == NULL)
    {
        errorMsg("Could not parse attachment string: \"%s\"",file_path_mime);
        exit(1);
    }

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

    a=(Attachment *) malloc(sizeof(Attachment));
    CHECK_MALLOC(a);

    a->file_path=xStrdup(file_path);
    a->file_name=xStrdup(file_name);

    switch (ntokens)
    {
        case 1: /* Only File_path/name given */
        {

            mime_type="application/octet-stream";
            a->mime_type=xStrdup(mime_type);
            a->content_disposition=xStrdup("attachment");
            break;
        }

        case 2: /* filepath/name, mime_type given */
        {
            mime_type=tokens[1];
            a->mime_type=xStrdup(mime_type);
            a->content_disposition=xStrdup("attachment");
            break;
        }

        case 3: /* filepath/name, mime_type, disposition given */
        {
            mime_type=tokens[1];
            a->mime_type=xStrdup(mime_type);
            content_disposition=tokens[2];
            if (*content_disposition == 'a')
                a->content_disposition=xStrdup("attachment");
            else if (*content_disposition == 'i')
                a->content_disposition=xStrdup("inline");
            else
                a->content_disposition=xStrdup("attachment");
            break;
        }
        
        case 4: /* filepath/name, mime_type, disposition, attchment name given */
        {
            mime_type=tokens[1];
            a->mime_type=xStrdup(mime_type);
            content_disposition=tokens[2];
            if (*content_disposition == 'a')
                a->content_disposition=xStrdup("attachment");
            else if (*content_disposition == 'i')
                a->content_disposition=xStrdup("inline");
            else
                a->content_disposition=xStrdup("attachment");

            a->attachment_name = xStrdup(tokens[3]);
            break;
        }

        default:
        {
            errorMsg("Invalid string specified with -a \"%s\"",file_path_mime);
            exit(1);
            break;
        }
    }
    na=allocateNode((void *) a);
    CHECK_MALLOC(na);

    appendNode(&attachment_head,&na);

    /*
    print_attachemtn_list();
    */
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

Sll *get_server_cap_list(void)
{
    return(server_caps);
}

/* just a debug routine */

