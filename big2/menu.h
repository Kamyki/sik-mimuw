//
// Created by Kamil Mykitiuk on 05/06/2018.
//

#ifndef BIG_MENU_H
#define BIG_MENU_H


#include <netinet/in.h>

typedef struct Station Station;

struct Station {
    Station* next;
    Station* prev;
    char name[65];
    struct sockaddr_in address;
    int socket;
    time_t time;
};


typedef struct {
    Station* stations;
    Station* active;
    int size;
} Menu;

Menu* create_menu();

size_t draw_menu(Menu* menu, uint8_t* buffer);

int transform(Menu* menu, uint8_t* buffer);

int add_menu(Menu* menu, struct sockaddr_in addr, char* name, time_t time);

void delete_menu(Menu* menu);
//TODO void remove()

Station active_station(Menu* menu);

const uint8_t* init_seq();

int check_time(Menu* menu, time_t time);

int menu_size(Menu* menu);

#endif //BIG_MENU_H
