//
// Created by Kamil Mykitiuk on 02/06/2018.
//

#ifndef BIG_ARGUMENT_PARSER_H
#define BIG_ARGUMENT_PARSER_H

#include <cstdint>

typedef struct {
    char MCAST_ADDR[64];
    char DISCOVER_ADDR[64];
    int64_t DATA_PORT;
    int64_t CTRL_PORT;
    int64_t UI_PORT;
    int64_t PSIZE;
    int64_t BSIZE;
    int64_t FSIZE;
    int64_t RTIME;
    char NAZWA[65];
} parameters;

void parseArgumentsReciver(int argc, char* argv[], parameters*);

void parseArgumentsTransmiter(int argc, char* argv[], parameters*);


#endif //BIG_ARGUMENT_PARSER_H
