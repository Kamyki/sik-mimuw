//
// Created by Kamil Mykitiuk on 08/06/2018.
//

#ifndef BIG_BUFFOR_H
#define BIG_BUFFOR_H

#include "stdint.h"


typedef struct Package Package;
struct Package {
    Package* next;
    Package* prev;
    void* data;
    uint64_t num;
};


typedef struct SetElem SetElem;
struct SetElem{
    SetElem* next;
    SetElem* prev;
    Package* package;
};


typedef struct {
    Package* packages;
    size_t fill;
    size_t size;
    uint64_t max_data;
    SetElem* set;
} Buffor;



void reset_buffor(Buffor* buffor);

Buffor* create_buffor(uint64_t size);

void add_to_buffor(Buffor* buffor, uint64_t num, void* data, size_t len);

void delete_set(Buffor* buffor);

void delete_buffor(Buffor* buffor);
Package* pop_from_buffor(Buffor* buffor);
void remove_package(Package* package);


#endif //BIG_BUFFOR_H
