//
// Created by Kamil Mykitiuk on 02/06/2018.
//

#include <cstdlib>
#include <cstring>
#include "fifo.h"
#include "err.h"





Package::Package(const std::vector<uint8_t>& data) : data(data) {}

Package::Package(uint8_t* data, uint64_t len, uint64_t num) {
    this->data = std::vector<uint8_t>(data, data+len);
    this->num = num;
}

void Fifo::add_to_queue(uint8_t* data, int64_t len, uint64_t num) {
    list.emplace_back(data, len, num);
    if(list.size() > max_count) {
        list.pop_front();
    }
}

Fifo::Fifo(uint64_t byte_size, uint64_t one_pack_size): one_pack_size(one_pack_size) {
    max_count = byte_size / one_pack_size;

}

std::vector<uint8_t> Fifo::retrieve(int64_t size, u_int64_t num) {
    for(auto pack: list) {
        if(pack.num == num)
            return pack.data;
    }
    return std::vector<uint8_t>();
}

Fifo::Fifo() {}

