/*
**  Appends a node to the end of a list sorted. The data of two lists are
**  passed to the user defined function compFunction for sorting. 
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-09-1998    first cut
*/

#include <sll.h>

typedef struct _addr
{
    char
        *name,
        *city,
        *state;
} Addr;

static void freeData(void **data);
static int compFunc(void *a1,void *a2);

int main (int argc,char **argv) 
{
    Sll
        *l,
        *head=NULL,
        *new=NULL;
    Addr
        *addr;

    int
        n=0;

    (void) fprintf(stderr,
"=========================================================================\n");
    (void) fprintf(stderr," Testing Append a node at the beginning of a list\n");
    (void) fprintf(stderr,
"=========================================================================\n");

    addr=(Addr *) malloc(sizeof(Addr));
    if (addr == NULL)
    {
        (void) fprintf(stderr," malloc failed\n");
        exit(-1);
    }

    (void) fprintf(stderr,"\n---------------[ appending ]----------\n");
    /*
    ** it will be the last node
    */
    addr->name=strdup("Cindy Muquit");
    addr->city=strdup("Philadelphia");
    addr->state=strdup("PA");

    if ((addr->name == NULL) ||
        (addr->city == NULL) ||
        (addr->state == NULL))
    {
        (void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }

    new=allocateNode((void *) addr);
    appendNodeSorted(&head,&new,compFunc);
    (void) fprintf(stderr,"Appending Node: %d\n", ++n);
    (void) fprintf(stderr,"  %s\n",addr->name);
    (void) fprintf(stderr,"  %s\n",addr->city);
    (void) fprintf(stderr,"  %s\n",addr->state);

    /*
    ** append node before the last one
    */
    addr=(Addr *) malloc(sizeof(Addr));
    if (addr == NULL)
    {
        (void) fprintf(stderr," malloc failed\n");
        exit(-1);
    }
    addr->name=strdup("Janet Hunter");
    addr->city=strdup("Santa Clara");
    addr->state=strdup("CA");

    if ((addr->name == NULL) ||
        (addr->city == NULL) ||
        (addr->state == NULL))
    {
        (void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }   
    new=allocateNode((void *) addr);
    appendNodeSorted(&head,&new,compFunc);
    (void) fprintf(stderr,"Appending Node: %d\n", ++n);
    (void) fprintf(stderr,"  %s\n",addr->name);
    (void) fprintf(stderr,"  %s\n",addr->city);
    (void) fprintf(stderr,"  %s\n",addr->state);


    /*
    ** append node before the last one
    */
    addr=(Addr *) malloc(sizeof(Addr));
    if (addr == NULL)
    {
        (void) fprintf(stderr," malloc failed\n");
        exit(-1);
    }
    addr->name=strdup("Babs Jensen");
    addr->city=strdup("Cupertino");
    addr->state=strdup("CA");

    if ((addr->name == NULL) ||
        (addr->city == NULL) ||
        (addr->state == NULL))
    {
        (void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }   
    new=allocateNode((void *) addr);
    appendNodeSorted(&head,&new,compFunc);
    (void) fprintf(stderr,"Appending Node: %d\n", ++n);
    (void) fprintf(stderr,"  %s\n",addr->name);
    (void) fprintf(stderr,"  %s\n",addr->city);
    (void) fprintf(stderr,"  %s\n",addr->state);

    /*
    ** print
    */
    (void) fprintf(stderr,"\n---------------[ printing ]----------\n");
    n=0;
    for (l=head; l; l=l->next)
    {
        addr=(Addr *) l->data;
        (void) fprintf(stderr,"Node: %d\n",++n);
        (void) fprintf(stderr,"  %s\n",addr->name);
        (void) fprintf(stderr,"  %s\n",addr->city);
        (void) fprintf(stderr,"  %s\n",addr->state);
    }

    /*
    ** free nodes
    */
    (void) fprintf(stderr,"\n---------------[ freeing ]----------\n");
    destroyNodes(&head,freeData);

    exit(0);
}


/*
** routine to free the user data
*/

static void freeData(void **data)
{
    Addr
        **addr=(Addr **) data;

    static int
        n=0;

    n++;
    if (*addr)
    {
        if ((*addr)->name)
        {
            (void) fprintf(stderr," Freeing: %s\n",(*addr)->name);
            (void) free((char *) (*addr)->name);
        }
        if ((*addr)->city)
        {
            (void) fprintf(stderr," Freeing: %s\n",(*addr)->city);
            (void) free ((char *) (*addr)->city);
        }
        if ((*addr)->state)
        {
            (void) fprintf(stderr," Freeing: %s\n",(*addr)->state);
            (void) free ((char *) (*addr)->state);
        }

        (void) fprintf(stderr,"Freeing the node %d itself\n\n",n);
        (void) free((char *) (*addr));
        (*addr)=NULL;
    }
}


static int compFunc(void *a1,void *a2)
{
    Addr
        *addr1=(Addr *) a1,
        *addr2=(Addr *) a2;

/*
    (void) fprintf(stderr,"name1=%s\n",addr1->name);
    (void) fprintf(stderr,"name2=%s\n",addr2->name);
*/
#ifdef WINNT
    return (stricmp(addr1->name,addr2->name));
#else
    return (strcasecmp(addr1->name,addr2->name));
#endif /* ! WINNT */
}

