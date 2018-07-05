//
// Created by Kamil Mykitiuk on 02/06/2018.
//

#include <stdlib.h>
#include <string.h>
#include "fifo.h"
#include "err.h"


queue* create_queue(int64_t size) {
    queue* fifo = malloc(sizeof(queue));
    fifo->fifo = malloc(sizeof(char) * size);
    if (fifo->fifo == NULL)
        syserr("malloc");
    fifo->last = 0;
    fifo->size = size;
    fifo->last_data_num = 0;
    return fifo;
}

void delete_queue(queue* fifo) {
    free(fifo->fifo);
}

void add_to_queue(queue* fifo, uint8_t* data, int64_t len) {
    if (len > fifo->size) {
        memcpy(fifo->fifo, data + (len - fifo->size), fifo->size);
        fifo->last = len - 1;
        fifo->last_data_num += len;
    } else if (len <= fifo->size - fifo->last) {
        memcpy(fifo->fifo + fifo->last, data, len);
        fifo->last += len;
        fifo->last_data_num += len;
    } else {
        memcpy(fifo->fifo + fifo->last, data, fifo->size - fifo->last);
        memcpy(fifo->fifo, data - (fifo->size - fifo->last), len - (fifo->size - fifo->last));
        fifo->last = len - (fifo->size - fifo->last);
        fifo->last_data_num += len;
    }
}

int retrieve(queue *fifo, uint8_t *buffer, int64_t size, u_int64_t num) {
    if (num + size > fifo->last_data_num || num < fifo->last_data_num - fifo->size)
        return -1;

    /*
     * padding = distance between end of data block to retrieve and last saved element
     *               V
     * ..._______|xxx|___|____||
     *                   ^
     *       last saved byte
     * */
    u_int64_t padding = num + size - fifo->last_data_num;

    if(padding < fifo->last) {
        char* start = fifo->fifo + fifo->last - padding - size;
        memcpy(start, buffer, size);
    } else {
        char* start = fifo->fifo + fifo->size - (size - fifo->last);
        memcpy(start, buffer, size - fifo->last);
        memcpy(fifo->fifo, buffer + (size - fifo->last), fifo->last);
    }
    return 0;
}
