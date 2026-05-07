#include "LazyList.h"
#include <cstdlib>

#include <iostream>

using namespace std;

bool LazyList::insert(int order_id) {
    Orders *pred,*curr;
    bool result;
    bool return_flag=0;
    while(true){
        pred=head;
        curr=pred->next;
        while(curr->order_id<order_id&&curr!=NULL){
            pred=curr;
            curr=curr->next;
        }
        pthread_mutex_lock(&pred->lock);
        pthread_mutex_lock(&curr->lock);
        if(validate(pred,curr)){
            if(order_id==curr->order_id){
                result= false;
                return_flag= true;
            }else{
                Orders *new_node=new Orders(order_id);
                if(!new_node){
                    return false;
                }
                new_node->next=curr;
                pred->next=new_node;
                result= true;
                return_flag= true;
            }
        }
        pthread_mutex_unlock(&pred->lock);
        pthread_mutex_unlock(&curr->lock);
        if(return_flag){
            return result;
        }
    }

}

int LazyList::search(int order_id) {
    Orders *curr;
    bool result;
    curr=head;
    while (curr->order_id<order_id){
        curr=curr->next;
    }
    if (!curr->marked && order_id == curr->order_id){
        return order_id;
    }else return -1;
}

bool LazyList::remove(int order_id) {
    Orders *pred,*curr;
    bool result;
    bool return_flag=0;
    while(true){
        pred=head;
        curr=pred->next;
        while(curr->order_id<order_id){
            pred=curr;
            curr=curr->next;
        }
        pthread_mutex_lock(&pred->lock);
        pthread_mutex_lock(&curr->lock);
        if(validate(pred,curr)){
            if(order_id==curr->order_id){
                curr->marked= true;
                pred->next=curr->next;
                result= true;
            }else result= false;
            return_flag=1;

        }
        pthread_mutex_unlock(&pred->lock);
        pthread_mutex_unlock(&curr->lock);
        if(return_flag==1) return result;

    }
}

bool LazyList::validate(LazyList::Orders *pred, LazyList::Orders *curr) {
    if(!pred->marked && !curr->marked && pred->next == curr)return true;
    else return false;
}
/*to be removed*/
int main(){
    LazyList list;
    bool result;
    list.insert(10);
    list.insert(5);
    list.insert(12);



    result=list.remove(12);


    return result;
}