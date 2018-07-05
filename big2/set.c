#include <string.h>
#include <stdbool.h>
#include "set.h"
#include "err.h"

set* create_set() {
    set* collection = malloc(sizeof(set));
    collection->first = NULL;
    return collection;
}

void delete_set(set* collection) {
    while (collection->first != NULL) {
        node* ele = collection->first;
        collection->first = ele->next;
        free(ele);
    }
    free(collection);
}

void add_to_set(set* collection, char* commaDelimited) {
    char* pt;
    pt = strtok(commaDelimited, ",");
    while (pt != NULL) {
        int64_t num = strtol(pt, NULL, 10);
        if(num >= 0) {
            bool found = false;
            if(collection->first == NULL) {
                collection->first = malloc(sizeof(node));
                collection->first->data_num = (u_int64_t) num;
                collection->first->next = NULL;
            } else {
                node* ele = collection->first;
                found = found | (ele->data_num == (u_int64_t) num);
                while(ele->next != NULL && !found) {
                    ele = ele->next;
                    found = found | (ele->data_num == (u_int64_t) num);
                }
                if(!found) {
                    ele->next = malloc(sizeof(node));
                    ele->next->data_num = (u_int64_t) num;
                    ele->next->next = NULL;
                }
            }
        }
        pt = strtok(NULL, ",");
    }
}

u_int64_t pop_set(set* collection) {
   node* ele = collection->first;
    if(ele == NULL)
        syserr("POP");
    collection->first = ele->next;
    u_int64_t a = ele->data_num;
    free(ele);
    return a;
}

bool empty_set(set* collection) {
    return collection->first == NULL;
}
