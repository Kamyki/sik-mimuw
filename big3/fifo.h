//
// Created by Kamil Mykitiuk on 02/06/2018.
//

#ifndef BIG_FIFO_H
#define BIG_FIFO_H

#include <cstdint>

#include <list>
#include <vector>

typedef struct {
    char* fifo;
    uint64_t last;
    uint64_t size;
    uint64_t last_data_num;
} queue;

struct Package {
    std::vector<uint8_t> data;
    uint64_t num;
    Package(const std::vector<uint8_t>& data);
    Package(uint8_t data[], uint64_t len, uint64_t num);
};

struct Fifo {

    std::list<Package> list;
    uint64_t one_pack_size;
    uint64_t max_count;
    uint64_t first_data_num;
    uint64_t last_data_num;

    Fifo();

    Fifo(uint64_t byte_size, uint64_t one_pack_size);

    std::vector<uint8_t> retrieve(int64_t size, u_int64_t num);

    void add_to_queue(uint8_t* data, int64_t len, uint64_t num);
};



#endif //BIG_FIFO_H
