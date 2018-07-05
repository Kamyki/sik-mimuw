//
// Created by Kamil Mykitiuk on 08/06/2018.
//

#ifndef BIG_BUFFOR_H
#define BIG_BUFFOR_H

#include <cstdint>
#include <list>
#include <map>
#include <vector>

struct Package {
    std::vector<uint8_t> data;
    uint64_t num;

    Package(uint8_t data[], size_t len, uint64_t num) : num(num) {
        this->data = std::vector<uint8_t>(data, data + len);
    }

    Package(uint64_t num) : num(num) {}
};

struct Buffer {
    std::list<Package> packages;
    std::map<uint64_t, std::list<Package>::iterator> set;
    size_t size;
    uint64_t max_data;

    void add_to_buffor(uint64_t num, uint8_t data[], size_t len);

    Package pop();

    void reset();
};


#endif //BIG_BUFFOR_H
