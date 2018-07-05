//
// Created by Kamil Mykitiuk on 04/06/2018.
//

#ifndef BIG_SET_H
#define BIG_SET_H


#include <stdlib.h>

typedef struct node node;

struct node {
    node* next;
    u_int64_t data_num;
};



typedef struct {
    node* first;
} set;

set* create_set();

void delete_set(set* collection);

void add_to_set(set* collection, char* commaDelimited);

u_int64_t pop_set(set* collection);

bool empty_set(set* collection);


#endif //BIG_SET_H
