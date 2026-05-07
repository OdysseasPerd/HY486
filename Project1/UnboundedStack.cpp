#include "UnboundedStack.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <time.h>
#include <iostream>
#include <sys/time.h>
#include <cassert>


#define THREADS 4
#define OPS_PER_THREAD 10

enum SlotState { EMPTY = 0, WAITING = 1, BUSY = 2 };
long getMicros() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void init_stack(STACK *s) {
    s->top = NULL;
    for (int i = 0; i < ELIM_CAPACITY; i++) {
        s->exchanger[i].value = 0;
        s->exchanger[i].state = EMPTY;
    }
    srand(time(NULL));
}

int backoff_delay() {
    return (rand() % (MAX_BACKOFF - MIN_BACKOFF + 1)) + MIN_BACKOFF;
}

void backoff() {
    usleep(backoff_delay());
}

int exchange(SLOT *slot, int my_value, long timeout_microsec) {
    long start = getMicros();
    if (__sync_bool_compare_and_swap(&slot->state, EMPTY, WAITING)) {
        slot->value = my_value;
        while (getMicros()-start < timeout_microsec) {
            if (slot->state == BUSY) {
                int other = slot->value;
                slot->state = EMPTY;
                return other;
            }
        }
        if (__sync_bool_compare_and_swap(&slot->state, WAITING, EMPTY)) {
            return TIMEOUT;
        } else {
            int other = slot->value;
            slot->state = EMPTY;
            return other;
        }
    } else if (slot->state == WAITING) {
        int other = slot->value;
        if(__sync_bool_compare_and_swap_4(&slot->state,WAITING,BUSY)){
            slot->value = my_value;
            return other;
        }

    }
    return TIMEOUT;
}

int visit(STACK *s, int my_value, int range, long duration) {
    int index = rand() % range;
    return exchange(&s->exchanger[index], my_value, duration);
}

int try_push(STACK *s, NODE *n) {
    NODE *old_top = s->top;
    n->next = old_top;
    return __sync_bool_compare_and_swap(&s->top, old_top, n);
}

NODE* try_pop(STACK *s) {
    NODE *old_top = s->top;
    if (old_top == NULL) return NULL;
    NODE *new_top = old_top->next;
    if (__sync_bool_compare_and_swap(&s->top, old_top, new_top)) {
        return old_top;
    }
    return NULL;
}

void push(STACK *s, int value) {
    NODE *n = new(NODE );
    n->order_id = value;
    n->next = NULL;

    while (1) {
        if (try_push(s, n)) return;

        int other = visit(s, value, ELIM_CAPACITY, 1000);
        if (other != TIMEOUT) {
            free(n);
            return;
        } else {
            backoff();
        }
    }
}

int pop(STACK *s) {
    NODE *n;
    while (1) {
        n = try_pop(s);
        if (n != NULL) {
            int v = n->order_id;
            free(n);
            return v;
        }

        int other = visit(s, 0, ELIM_CAPACITY, 1000);
        if (other != TIMEOUT) {
            return other;
        } else {
            backoff();
        }
    }
}
/*To be removed
STACK stack;

void verify_stack_operations() {
    printf("=== Basic Stack Test ===\n");
    STACK s;
    init_stack(&s);

    // Test push/pop
    push(&s, 42);
    push(&s, 99);
    cout<<"1\n";
    assert(pop(&s) == 99);
    cout<<"1\n";
    assert(pop(&s) == 42);
    cout<<"1\n";
    //assert(pop(&s) == TIMEOUT);  // Empty test
    cout<<"1\n";
    printf("Basic tests passed!\n");
}

// --- Threaded Tests ---
#define THREAD_ITEMS 1000
STACK shared_stack;

void* producer(void* arg) {
    for (int i = 0; i < THREAD_ITEMS; i++) {
        push(&shared_stack, i);
    }
    return NULL;
}

void* consumer(void* arg) {
    int *count = (int*)arg;
    for (int i = 0; i < THREAD_ITEMS; i++) {
        while (pop(&shared_stack) == TIMEOUT) {} // Retry on empty
        (*count)++;
    }
    return NULL;
}

void test_thread_safety() {
    printf("\n=== Thread Safety Test ===\n");
    init_stack(&shared_stack);
    pthread_t producer_thread, consumer_thread;
    int consumed = 0;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, &consumed);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    assert(consumed == THREAD_ITEMS);
    printf("Thread test passed! Items consumed: %d\n", consumed);
}

// --- Elimination Test ---
void* elimination_producer(void* arg) {
    // Slow producer (forces elimination)
    for (int i = 0; i < THREAD_ITEMS; i++) {
        push(&shared_stack, i);
        usleep(100);  // Delay to encourage elimination
    }
    return NULL;
}

void test_elimination() {
    printf("\n=== Elimination Backoff Test ===\n");
    init_stack(&shared_stack);
    pthread_t producer_thread, consumer_thread;
    int consumed = 0;

    pthread_create(&producer_thread, NULL, elimination_producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, &consumed);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    assert(consumed == THREAD_ITEMS);
    printf("Elimination test passed! Items consumed: %d\n", consumed);
}

int main() {
    verify_stack_operations();     // Single-threaded validation
    test_thread_safety();          // Concurrent push/pop
    test_elimination();            // Verify elimination works
    printf("\nAll tests passed!\n");
    return 0;
}
*/

