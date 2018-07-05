//
// Created by Kamil Mykitiuk on 08/06/2018.
//

#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "buffor.h"
#include "err.h"

void Buffer::add_to_buffor(uint64_t num, void* data, size_t len) {
    auto found = this->set.find(num);
    if (found != this->set.end()) {
        found->second->data = std::vector(data, data +len);
        return;
    }

    if (this->packages.size() == 0) {
        this->add_first(num, data, len);
    } else {
        while (this->packages.size() < this->size) {
            if (this->packages.back().num + len == num) {
                Package pack(data, len, num);
                this->packages.push_back(pack);
                return;
            } else {
                Package dummy(num);
                this->packages.push_back(dummy);
                this->set[dummy.num] = --this->packages.end();
            }
            if (this->packages.size() == this->size) {
                Package a = this->packages.back();
                this->set.erase(a.num);
                this->packages.pop_back();
            }
        }

    }

}

Package Buffer::pop() {
    Package a = this->packages.front();
    this->packages.pop_front();
    return a;
}

