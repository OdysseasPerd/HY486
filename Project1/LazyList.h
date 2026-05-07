#include <pthread.h>
#ifndef PROJECT1_UNBOUNDEDLIST_H
#define PROJECT1_UNBOUNDEDLIST_H
class LazyList{
public:
    typedef struct node{
        int order_id;
        bool marked;
        pthread_mutex_t lock;
        node* next;
        node(int id) : order_id(id), marked(false), next(nullptr) {
            pthread_mutex_init(&lock, NULL);  // Initialize mutex
        }
        ~node(){
            pthread_mutex_destroy(&lock);
        }

    }Orders;
    Orders* head;
public:
    LazyList(){
        Orders*tail=new Orders(INT_MAX);
        Orders *dummy=new Orders (INT_MIN);
        dummy->next=tail;
        head=dummy;
    }
    bool insert(int order_id);
    int search(int order_id);
    bool remove(int order_id);
    bool validate(Orders* pred,Orders* curr);
};



#endif //PROJECT1_UNBOUNDEDLIST_H
