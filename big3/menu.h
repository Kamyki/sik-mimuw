//
// Created by Kamil Mykitiuk on 05/06/2018.
//

#ifndef BIG_MENU_H
#define BIG_MENU_H

extern "C" {
#include <netinet/in.h>

};

#include <list>
#include <string>
#include <utility>

struct Station {
    std::string name;
    struct sockaddr_in address;
    time_t time;

    Station(const std::string& name, const sockaddr_in& address, time_t time);
};


struct Menu {
    std::list<Station> stations;
    std::list<Station>::iterator active;
    std::string prefered_name;
    bool prefered_set;
    bool is_active = false;

    Menu();

    Menu(std::string prefered_name) : prefered_name(std::move(prefered_name)) {
        prefered_set = true;
    }

    std::vector<uint8_t> draw_menu();

    int add_menu(struct sockaddr_in addr, char* name, time_t time);

    int transform(uint8_t* buffer);

    int check_time(time_t time);

};


std::vector<uint8_t> init_seq();


#endif //BIG_MENU_H
