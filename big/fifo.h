//
// Created by Kamil Mykitiuk on 02/06/2018.
//

#ifndef BIG_FIFO_H
#define BIG_FIFO_H

#include <stdint.h>

typedef struct {
    char* fifo;
    uint64_t last;
    uint64_t size;
    uint64_t last_data_num;
} queue;

queue* create_queue(int64_t size);

void delete_queue(queue* fifo);

void add_to_queue(queue* fifo, uint8_t* data, int64_t len);

int retrieve(queue *fifo, uint8_t* buffer, int64_t size, u_int64_t num);

#endif //BIG_FIFO_H
