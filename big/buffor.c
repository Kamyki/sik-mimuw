//
// Created by Kamil Mykitiuk on 08/06/2018.
//

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "buffor.h"
#include "err.h"


Package* pop_from_buffor(Buffor* buffor);

void remove_package(Package* package);

Package* check_set(Buffor* buffor, uint64_t num);

void reset_buffor(Buffor* buffor) {
    while (buffor->packages != NULL) {
        Package* elem = buffor->packages;
        if (buffor->fill == 1) {
            buffor->fill--;
            buffor->packages = NULL;
        } else {
            buffor->packages = elem->next;
            elem->prev->next = elem->next;
            elem->next->prev = elem->prev;
            buffor->fill--;
        }
        remove_package(elem);

    }
    delete_set(buffor);
}

Buffor* create_buffor(uint64_t size) {
    Buffor* buffor = malloc(sizeof(Buffor));
    buffor->packages = NULL;
    buffor->size = 0;
    buffor->fill = 0;
    buffor->max_data = size;
    buffor->set = NULL;
    return buffor;
}

void add_to_set(Buffor* bufor, Package* package) {
    SetElem* setElem = malloc(sizeof(setElem));
    if (setElem == NULL)
        syserr("malloc");
    setElem->package = package;
    if (bufor->set == NULL) {
        setElem->prev = setElem;
        setElem->next = setElem;
        bufor->set = setElem;
    } else {
        setElem->next = bufor->set;
        setElem->prev = bufor->set->prev;
        bufor->set->prev->next = setElem;
        bufor->set->prev = setElem;
    }
    assert(package != NULL);
}

void datacpy(Package* pack, void* data, size_t len, uint64_t num) {
    pack->data = malloc(len);
    if (pack->data == NULL)
        syserr("malloc");
    memcpy(pack->data, data, len);
    pack->num = num;
}

void add_to_buffer_first(Buffor* buffor, Package* pack, size_t len) {
    buffor->size = buffor->max_data / len;
    pack->prev = pack;
    pack->next = pack;
    buffor->fill = 1;
    buffor->packages = pack;
}

void add_to_buffer_end(Buffor* buffor, Package* pack) {
    buffor->packages->prev->next = pack;
    pack->prev = buffor->packages->prev;
    buffor->packages->prev = pack;
    pack->next = buffor->packages;
    buffor->fill++;
}

void test(Buffor* buffor) {
    Package* ele = buffor->packages;
    if(ele == NULL)
        return;
    for(int i=0; i<buffor->size; ++i, ele=ele->next) {
        assert(ele->next != NULL);
        assert(ele->prev != NULL);
    }
}

void add_to_buffor(Buffor* buffor, uint64_t num, void* data, size_t len) {
    test(buffor);
    Package* found = check_set(buffor, num);
    if (found != NULL) {
        found->data = malloc(len);
        if (found->data == NULL)
            syserr("malloc");
        memcpy(found->data, data, len);
        return;
    }
    Package* pack = malloc(sizeof(Package));
    if (pack == NULL)
        syserr("malloc");
    datacpy(pack, data, len, num);

    if (buffor->packages == NULL) {
        add_to_buffer_first(buffor, pack, len);
    } else {
        while (buffor->fill < buffor->size) {
            if (buffor->packages->prev->num + len == num) {
                add_to_buffer_end(buffor, pack);
                break;
            } else {
                Package* dummy = malloc(sizeof(Package));
                if (dummy == NULL)
                    syserr("malloc");
                dummy->num = buffor->packages->prev->num + len;
                dummy->data = NULL;
                add_to_buffer_end(buffor, dummy);
                add_to_set(buffor, dummy);
            }
            if (buffor->fill == buffor->size) {
                Package* discard = pop_from_buffor(buffor);
                remove_package(discard);
            }
        }

    }
    test(buffor);
}

/**
 * Checks if set(list) of missing packages contains num, if so removes it and return
 * */
Package* check_set(Buffor* buffor, uint64_t num) {
    if (buffor->set == NULL) {
        return NULL;
    } else {
        SetElem* elem = buffor->set;
        do {
            if (elem->package->num == num) {
                if (elem->next == elem) { // 1 element
                    buffor->set = NULL;
                    Package* pack = elem->package;
                    free(elem);
                    return pack;
                } else {
                    elem->next->prev = elem->prev;
                    elem->prev->next = elem->next;
                    if (elem == buffor->set) {
                        buffor->set = elem->next;
                    }
                    Package* pack = elem->package;
                    free(elem);
                    return pack;
                }
            }
            elem = elem->next;
        } while (elem != buffor->set && elem->package->num < num);
        return NULL;
    }
}

void remove_package(Package* package) {
    if (package->data != NULL)
        free(package->data);
    free(package);
}

Package* pop_from_buffor(Buffor* buffor) {
    if (buffor->fill == 0)
        syserr("Empty buffer");
    Package* temp = buffor->packages;
    if (buffor->fill == 1) {
        buffor->packages = NULL;
    } else {
        buffor->packages = buffor->packages->next;
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }
    buffor->fill--;
    check_set(buffor, temp->num);
    return temp;
}

void delete_set(Buffor* buffor) {
    while (buffor->set != NULL) {
        SetElem* setElem = buffor->set;
        if (setElem->next == setElem) {
            free(setElem);
            buffor->set = NULL;
        } else {
            buffor->set = setElem->next;
            setElem->prev->next = setElem->next;
            setElem->next->prev = setElem->prev;
            free(setElem);
        }
    }
}

void delete_buffor(Buffor* buffor) {
    reset_buffor(buffor);
    free(buffor);
}
