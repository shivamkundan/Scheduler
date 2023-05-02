//Shivam Kundan
//ECE493 Spring '16
//Lab5

/*Implementation of a first in first out queue that holds pid's.*/

#include <stdio.h>
#include <stdlib.h>

typedef struct list_tag
/*Structure for nodes in the queue.*/
{
    pid_t pid;
    struct list_tag* next;  //pointer to next node
} ListNode;


typedef struct
/*Structure for the queue.*/
{
    ListNode* first;
    ListNode* last;
} queue;


void queueInit(queue * qp)
/*Initializes new queue.*/
{
    qp->first = NULL;
    qp->last = NULL;
}


void queueInsert(queue * qp, pid_t pid)
/*Inserts pid into queue.*/
{
    ListNode* new_node = (ListNode*) malloc(sizeof(ListNode));

    if (new_node == NULL)
        {
            printf("Out of memory\n");
            exit(1);
        }

    //Assign node values
    new_node->pid=pid;
    new_node->next= NULL;

    //If new queue
    if (qp->last == NULL)
        {
            qp->first = qp->last = new_node;
        }

    //For existing queues
    else
        {
            qp->last->next= new_node;
            qp->last = new_node;
        }
}


void queuePop(queue *qp)
/*Removes first item from queue. Returns its pid.*/
{
    ListNode *old_first = (ListNode*) malloc(sizeof(ListNode));
    old_first=qp->first;

    //Set new first node
    qp->first = qp->first->next;

    //return pid of old node
    printf("(removed pid: %d)\n",old_first->pid);

    free(old_first);
}


void queueRotate(queue *qp)
/*Pushes first element to the back of the queue.*/
{
    ListNode *old_first, *old_last = (ListNode*) malloc(sizeof(ListNode));
    old_first = qp->first;
    old_last = qp->last;

    qp->first=old_first->next;

    qp->last->next=old_first;
    qp->last=old_first;
    old_first->next=NULL;
}


void queuePrint(queue q)
/*Prints contents of queue from first to last.*/
{
    ListNode* n;
    for (n = q.first; n != NULL; n = n->next)
        {
            printf("pid:%d\n", n->pid);
        }

}

