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

    Package(void* data, size_t len, uint64_t num) : num(num) {
        this->data = std::vector(data, data + len);
    }

    Package(uint64_t num) : num(num) {}
};

class Buffer {
    std::list<Package> packages;
    std::map<uint64_t, std::list<Package>::iterator> set;
    size_t size;
    uint64_t max_data;

private:
public:

    void add_to_buffor(uint64_t num, void* data, size_t len);

    Package pop();
};


#endif //BIG_BUFFOR_H
