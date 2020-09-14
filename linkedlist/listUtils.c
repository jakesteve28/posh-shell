#include "listUtils.h"

Node * buildNode( FILE * in, void *(*buildData)(FILE * fin) )
{
    Node * nn = (Node*)calloc(1, sizeof(Node));
    nn->data = buildData(in);
    return nn;
}// end buildNode

Node * buildNode_Type(void * passedIn)
{
    Node * nn = (Node*)calloc(1, sizeof(Node));
    nn->data = passedIn;
    return nn;
}// end buildNode_Type


void buildListTotal(LinkedList * myList, int total, FILE * fin, void * (*buildData)(FILE * in) )
{
    for(int i = 0 ; i < total ; i++)
    {
        Node * nn = buildNode(fin, buildData);
        addFirst(myList, nn);
    }
}// end buildListTotal


void sort(LinkedList * theList, int (*compare)(const void *, const void *))
{
    Node * start, *search, *min;
    void * temp;
    if(theList == NULL)
    {
        exit(-99);
    }
    if(theList->size > 1)
    {

        for(start = theList->head->next; start->next != NULL; start = start->next)
        {
            min = start;

            for(search = start->next; search != NULL; search = search->next)
                if(compare(search->data, min->data) < 0)
                    min = search;

            temp = start->data;
            start->data = min->data;
            min->data = temp;

        }// end for start

    }// end if
}// end sort
