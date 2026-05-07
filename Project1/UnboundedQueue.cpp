#include "UnboundedQueue.h"
#include <iostream>


void UnboundedQueue::init(UnboundedQueue::queue *queue1) {
    NODE *p=new(NODE);
    p->next= nullptr;
    queue1->head=queue1->tail=p;
}

void UnboundedQueue::enqueue(UnboundedQueue::queue *queue1, int order_id) {
    NODE *next,*last;
    NODE *p=new(NODE);
    p->order_id=order_id;
    p->next= nullptr;
    while(true){
        last=queue1->tail;
        next=last->next;
        if(last==queue1->tail){
            if(next== nullptr){
                if(__sync_bool_compare_and_swap(&last->next,next,p)){
                    break;
                }
            }else __sync_bool_compare_and_swap(&queue1->tail,last,next);
        }
    }
    __sync_bool_compare_and_swap(&queue1->tail,last,p);
}

int UnboundedQueue::dequeue(UnboundedQueue::queue *queue1) {
    NODE *first,*last,*next;
    int temp;

    while(true){
        first=queue1->head;
        last=queue1->tail;
        next=first->next;
        if (first==queue1->head){
            if (first==last){
                if(next== nullptr){
                    return -1;
                }
                __sync_bool_compare_and_swap(&queue1->tail,last,next);
            }else{
                temp=next->order_id;
                if(__sync_bool_compare_and_swap(&queue1->head,first,next)){
                    break;
                }
            }
        }
    }

    return temp ;//order_id;
   // else return -1;
}
