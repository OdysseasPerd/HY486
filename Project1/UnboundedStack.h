
#ifndef PROJECT1_UNBOUNDEDSTACK_H
#define PROJECT1_UNBOUNDEDSTACK_H
#include <ctime>
#include <cstdlib>
#include <optional>
using namespace std;
const int ELIMINATION_ARRAY_SIZE=8;


#define EMPTY_STACK -1
#define TIMEOUT -999999
#define MIN_BACKOFF 5
#define MAX_BACKOFF 15
#define ELIM_CAPACITY 10

typedef struct node {
    int order_id;
    struct node *next;
} NODE;

typedef struct slot {
    int value;
    int state;
} SLOT;

typedef struct stack {
    NODE *top;
    SLOT exchanger[ELIM_CAPACITY];
} STACK;

void init_stack(STACK *s);
void push(STACK *s, int value);
int pop(STACK *s);

#endif //PROJECT1_UNBOUNDEDSTACK_H
