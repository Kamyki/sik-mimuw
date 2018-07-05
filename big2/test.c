//
// Created by Kamil Mykitiuk on 09/06/2018.
//
#include <arpa/inet.h>
#include <memory.h>

int main() {
    uint8_t* pam[200];
    memset(pam, '\0', 200);

    uint64_t a =128;

    uint64_t b = htonll(a);
    memcpy(pam, &b, 8);
    uint64_t d = 0;
    memcpy(pam + 8, &d, 8);

    uint64_t c = ntohll(*(uint64_t*)pam);

}