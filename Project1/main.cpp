
#include <cstddef>
#include <cstdio>
#include <string>
#include "UnboundedStack.h"
#include "LazyList.h"
#include "UnboundedQueue.h"
#include <pthread.h>
#include <iostream>

using namespace std;
#ifndef N_THREADS
#define N_THREADS 4  // Default value, if Makefile does not provide any
#endif
#define DIST (N_THREADS / 2)
#define AGNT (N_THREADS / 4)
#define COOK (N_THREADS / 4)

static_assert(N_THREADS > 0);
static_assert((N_THREADS % 4) == 0);

using Order = size_t;

/*struct PendingOrdersStack {
    // TODO
};
PendingOrdersStack PendingOrders;

struct UnderPreparationOrdersQueue {
    // TODO
};*/

/*
struct CompletedOrdersList {
    // TODO
};*/
STACK stack;
UnboundedQueue UnderPreparationOrders;
struct District {
    LazyList completedOrders[DIST];
    size_t checksum;
};
District Districts[DIST];

std::string PassStr(bool pass) {return (pass ? "PASS" : "FAIL");}
void PrintPendingOrdersEmpty(bool pass, size_t n) {
    const std::string passStr = PassStr(pass);
    printf("%s PendingOrders Empty %lu\n", passStr.data(), n);
}
void PrintUnderPreparationOrdersEmpty(bool pass, size_t n) {
    const std::string passStr = PassStr(pass);
    printf("%s UnderPreparationOrders Empty %lu\n", passStr.data(), n);
}
void PrintCompletedOrdersEmpty(bool pass, size_t tid, size_t n) {
    const std::string passStr = PassStr(pass);
    printf("%s District[%lu].completedOrders Empty %lu\n", passStr.data(), tid, n);
}
void PrintCompletedOrdersSize(bool pass, size_t tid, size_t sum) {
    const std::string passStr = PassStr(pass);
    printf("%s District[%lu].completedOrders Size %lu\n", passStr.data(), tid, sum);
}
void PrintCompletedOrdersValid(bool pass, size_t tid, size_t checksum) {
    const std::string passStr = PassStr(pass);
    printf("%s District[%lu].completedOrders Valid %lu\n", passStr.data(), tid, checksum);
}

void * DistrictThread(void *arg){
    int tid=*(int*)arg;
    for (int idx=0;idx<DIST;idx++) {
        int order_id=(tid*DIST)+idx;
        push(&stack,order_id);
        cout<<"Pushed %d\n"<<order_id;
    }
    return nullptr;
}

int main() {
    //TODO
    pthread_t district_threads[DIST];
    int district_tids[DIST];
    init_stack(&stack);
    for(int tid=0;tid<DIST;tid++){
        district_tids[tid]=tid;
        pthread_create(&district_threads[tid], nullptr,DistrictThread,&district_tids[tid]);
    }
    printf("%d\n", N_THREADS);
}
