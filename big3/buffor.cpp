//
// Created by Kamil Mykitiuk on 08/06/2018.
//

#include <cstdlib>
#include <memory.h>
#include "buffor.h"
#include "err.h"

void Buffer::add_to_buffor(uint64_t num, uint8_t data[], size_t len) {

    auto found = this->set.find(num);
    if (found != this->set.end()) {
        found->second->data = std::vector<uint8_t>(data, data +len);
        set.erase(found);
        return;
    }

    if (this->packages.empty()) {
        this->size = this->max_data / len;
        this->packages.emplace_back(data, len, num);
    } else {
        while (this->packages.size() < this->size) {
            if(num < packages.front().num)
                return;
            if (this->packages.back().num + len == num) {
                this->packages.emplace_back(data, len, num);
                return;
            } else {
                this->packages.emplace_back(this->packages.back().num + len);
                this->set[packages.back().num] = --this->packages.end();
            }
            if (this->packages.size() == this->size) {
                uint64_t distance = (num - packages.back().num)/len < size -1 ?
                        (num - packages.back().num)/len : size -1;
                auto it = packages.begin();
                std::advance(it, -distance);
                auto el = set.lower_bound(it->num);
                set.erase(set.begin(), el);
                this->packages.pop_front();
            }
        }

    }

}

Package Buffer::pop() {
    Package a = this->packages.front();
    this->packages.pop_front();
    return a;
}

void Buffer::reset() {
    this->set.clear();
    this->packages.clear();
}

