#include "linkedList.h"

LinkedList * linkedList()
{
    LinkedList* ret = (LinkedList*)calloc(1, sizeof(LinkedList));
    ret->head = (Node*)calloc(1, sizeof(Node));
    ret->size = 0;
    return ret;
}// end linkedList

void clearList(LinkedList * theList, void (*removeData)(void *))
{
    if(theList == NULL)
    {
        return;
    }
    if(theList->head->next == NULL)
    {
        return;
    }
    Node * cur = theList->head->next;
    Node * temp = NULL;
    while(cur != NULL)
    {
        temp = cur;
        cur = cur->next;
        removeData(temp->data);
        free(temp);
        temp = NULL;
    }
    theList->head->next = NULL;
    theList->head->prev = NULL;
    theList->size = 0;
    free(theList->head);
    theList->head = NULL;
    free(theList);
    theList = NULL;
}// end clearList
void printList(const LinkedList * theList, void (*convertData)(void *))
{
    if(theList == NULL)
    {
        printf("Empty List\n");
        return;
    }
    if(theList->head->next == NULL)
    {
        printf("Empty List\n");
        return;
    }
    Node * iter = theList->head->next;
    while(iter != NULL)
    {
        convertData(iter->data);
        iter = iter->next;
    }
}// end printList
void addLast(LinkedList * theList, Node * nn)
{
    if(theList == NULL)
    {
        return;
    }
    if(theList->head->next == NULL)
    {
        addFirst(theList, nn);
        return;
    }
    Node * cur = theList->head->next;
    while(cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = nn;
    nn->prev = cur;
    return; //lol
}// end addLast

void addFirst(LinkedList * theList, Node * nn)
{
    if(theList == NULL)
    {
        return;
    }
    if(nn == NULL)
    {
        return;
    }
    Node * addThis = nn;
    if(theList->head->next == NULL)
    {
        theList->head->next = addThis;
        addThis->prev = theList->head;
        theList->size++;
        return;
    }
    Node * moveUp = theList->head->next;
    theList->head->next = addThis;
    addThis->prev = theList->head;
    addThis->next = moveUp;
    moveUp->prev = addThis;
    theList->size++;
}// end addFirst

void removeFirst(LinkedList * theList, void (*removeData)(void *))
{
    if(theList == NULL)
    {
        return;
    }
    if(theList->head->next == NULL)
    {
        return;
    }
    Node * deleteThis = theList->head->next;
    if(deleteThis->next == NULL)
    {
        removeData(deleteThis->data);
        deleteThis->prev->next = NULL;
        deleteThis->prev = NULL;
        free(deleteThis);
        deleteThis = NULL;
        theList->size--;
        return;
    }
    Node * attachDummyHeadToThis =  deleteThis->next;
    theList->head->next = attachDummyHeadToThis;
    attachDummyHeadToThis->prev = theList->head;
    deleteThis->prev = NULL;
    deleteThis->next = NULL;
    removeData(deleteThis->data);
    free(deleteThis);
    deleteThis = NULL;
    theList->size--;
}// end removeFirst

void removeLast(LinkedList * theList, void (*removeData)(void *))
{
    if(theList == NULL)
    {
        return;
    }
    if(theList->head->next == NULL)
    {
        return;
    }
    Node * cur = theList->head->next;
    Node * prev = NULL;
    while(cur->next != NULL) //Iterates until cur is on the LAST node, and prev is on second to last.
    {
        prev = cur;
        cur = cur->next;
    }
    prev->next = NULL;
    cur->prev = NULL;
    theList->size--;
    removeData(cur->data);
    cur->data = NULL;
    free(cur);
    cur = NULL;
}// end removeLast


void removeItem(LinkedList * theList, void * (*buildType)(FILE * stream), void (*removeData)(void *), int (*compare)(const void *, const void *))
{
    if(theList == NULL)
    {
        return;
    }
    if(theList->head->next == NULL)
    {
        return;
    }
    int flag = 0;
    void * data = buildType(stdin);
    Node * prev = theList->head;
    Node * cur =  theList->head->next;
    while(cur != NULL)
    {
        if(compare(cur->data, data) == 0)
        {
            flag = 1;
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    if(flag == 1)
    {
        Node * bridge = cur->next;
        prev->next = bridge;
        if(bridge != NULL)
        {
            bridge->prev = prev;
        }
        cur->next = NULL;
        removeData(cur->data);
        cur->data = NULL;
        cur->prev = NULL;
        free(cur);
        cur = NULL;
        free(data);
        data = NULL;
        theList->size--;
    }
    else
    {
        printf("element not found");
        return;
    }
}


