//
// Created by perdhs on 10/04/2025.
//

#ifndef PROJECT1_UNBOUNDEDQUEUE_H
#define PROJECT1_UNBOUNDEDQUEUE_H


class UnboundedQueue {
public:
    typedef struct node{
        int order_id;
        struct node *next;
    }NODE;
    typedef struct queue{
        NODE *head;
        NODE *tail;
    }queue;
    void init(queue *queue1);
    void enqueue(queue *queue1,int order_id);
    int dequeue(queue *queue1);//int order_id);
};


#endif //PROJECT1_UNBOUNDEDQUEUE_H
