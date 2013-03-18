#include "sll.h"

/*
** private protos
static void freeList(Sll **list);
*/

/*
**  initList()
**  initialize a list
**
**  Parameters:
**  Sll **list      list to initialize
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  none
**
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/
void initList(Sll **list)
{
    (*list)=NULL;
}


/*
**  allocateNode()
**  allocate a new node.
**
**  Parameters:
**  void    *data       a generic pointer to object data
**
**  Return Values:
**  pointer to Sll if succeeds
**  NULL otherwise
**
**  Limitations and Comments:
**  the caller must pass valid pointer to data.
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/

Sll *allocateNode(void *data)
{
    char
        *func="allocateNode() in sll.c";

    Sll
        *sll;

    sll=(Sll *) malloc(sizeof(Sll));
    if (sll == (Sll *) NULL)
    {
        (void) fprintf(stderr,"malloc failed at: %s\n",func);
        return ((Sll *) NULL);
    }

    sll->data=data;
    sll->next=NULL;

    return (sll);
}

/*
**  appendNode()
**  appends a node to the end of a list
**
**  Parameters:
**  Sll **head      - modify the list
**  Sll **new       - appends this node
**
**  Return Values:
**  None
**
**  Limitations and Comments:
**  new node must be allocated and initialized before passing it here
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/

void appendNode(Sll **head,Sll **new)
{
    Sll
        *tmp;

    if (emptyList(*head) == TRUE)
    {
        (*head)=(*new);
    }
    else
    {
        for (tmp=(*head); tmp->next != NULL; tmp=tmp->next)
            ;
        tmp->next=(*new);
    }
}

/*
**  appendNodeSorted()
**  appends a node to the end of a list sorting by a user defined function
**
**  Parameters:
**  Sll **head      - append at the ends of this node
**  Sll **new       - appends this node
**
**  Return Values:
**  None
**
**  Limitations and Comments:
**  new node must be allocated and initialized before passing it here
**  the function takes two arguments, void * each
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/

void appendNodeSorted(Sll **head,Sll **new,Ifunc compFunc)
{
    Sll
        *tmp;

    if (emptyList(*head) == TRUE)
    {
        (*head)=(*new);
    }
    else
    {
        if ((*compFunc)((*head)->data,(*new)->data) > 0)
        {
            (*new)->next=(*head);
            (*head)=(*new);
        }
        else
        {
            for(tmp=(*head); tmp->next; tmp=tmp->next)
            {
                if ((*compFunc)(tmp->next->data,(*new)->data) > 0)
                    break;
            }
            (*new)->next=tmp->next;
            tmp->next=(*new);
        }
    }
}

/*
**  insertNode()
**  insert a node at the beginning of a list
**
**  Parameters:
**  Sll **head      - modify this list
**  Sll **new       - appends this node
**
**  Return Values:
**  None
**
**  Limitations and Comments:
**  new node must be allocated and initialized before passing it here
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/

void insertNode(Sll **head,Sll **new)
{

    (*new)->next=(*head);
    (*head)=(*new);
}

/*
**  emptyList()
**  check if a list variable is NULL
**
**  Parameters:
**  Sll *list      list
**
**  Return Values:
**  TRUE    if empty
**  FALSE   if not empty
**
**  Limitations and Comments:
**  list must be allocated/initialized or initialized before calling
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/

Bool emptyList(Sll *list)
{
    return ((list == NULL) ? TRUE : FALSE);
}

/*
**  delNode()
**  remove a node from a list
**
**  Parameters:
**  Sll **head      - list to modify
**  Sll *node       - node to remove
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  list is modified
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/
void delNode(Sll **head,Sll *node)
{
    if (emptyList(*head) == TRUE)
        return;

    if ((*head) == node)
        (*head)=(*head)->next;
    else
    {
        Sll
            *l;
        for (l=(*head); l != NULL && l->next != node; l=l->next);
        if (l == NULL)
            return;
        else
            l->next=node->next;
    }
    freeNode(&node);
}

/*
**  freeNode()
**  frees a node
**
**  Parameters:
**  Sll **list  node to free
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  if list is not null, it wil be freed. so list better point to a valid
**  location
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-07-1998    first cut
*/

void freeNode(Sll **list)
{
    if (*list)
    {
        (void) free ((char *) (*list));
        (*list)=NULL;
    }
}


/*
**  getNthNode()
**  get nth node in a list
**
**  Parameters:
**  Sll *list       - the head list
**  int n           - return the node
**  Return Values:
**  a pointer to the list at position n
**  NULL if there's no such node at posion n
**
**  Limitations and Comments:
**  position starts at 1
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-08-1998    frist cut
*/

Sll *getNthNode(Sll *list,int n)
{
    Sll
        *lp=NULL;
    int
        j=0;

    for (lp=list; lp; lp=lp->next)
    {
        j++;
        if (j == n)
        {
            return (lp);
        }
    }

    return ((Sll *) NULL);
}

/*
**  destroyNode()
**  frees a node and the data associated with it
**
**  Parameters:
**  Sll **list      - modify this list
**  SLl *node       - remove the node
**  void (*freeFunc)()  - pointer to function to free the data
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  none
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-08-1998    first cut
*/

/*
void destroyNode(Sll **list,Sll *node,void (*freeFunc)(void **))
*/
void destroyNode(Sll **list,Sll *node,Vfunc freeFunc)
{
    if (emptyList(node) == FALSE)
    {
        /*
        ** destroy the data
        */
        if (freeFunc != NULL)
            (*freeFunc) (&(node->data));

        delNode(list,node);
    }
}

/*
**  destroyNodes()
**  destroy the entire linked list and the data
**
**  Parameters:
**  Sll **head      - head node of the list
**  freeFunc        - function to free data
**
**  Return Values:
**  none
**
**  Limitations and Comments:
**  whole list and the data associated are freed
**
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-08-1998    first cut
*/

/*
void destroyNodes(Sll **head,void (*freeFunc)(void **))
*/
void destroyNodes(Sll **head,Vfunc freeFunc)
{
    Sll
        *lp;
    while (*head)
    {
        lp=(*head);
        if (freeFunc != NULL)
            (*freeFunc) (&lp->data);
        (*head)=(*head)->next;
        (void) free((char *) lp);
    }
}

/*
**  numNodes()
**  returns number of nodes in the list
**
**  Parameters:
**  Sll **head      - the head node of the list
**
**  Return Values:
**  number of node/s
**
**  Limitations and Comments:
**  traverse the whole list, so not efficient
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-09-1998    first cut
*/

int numNodes(Sll **head)
{
    int
        n=0;

    Sll
        *lp;

    for (lp=(*head); lp; lp=lp->next)
    {
        n++;
    }

    return (n);
}

