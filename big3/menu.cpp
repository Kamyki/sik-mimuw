//
// Created by Kamil Mykitiuk on 05/06/2018.
//

#include <cstring>
#include <cstdlib>
#include <vector>
#include "menu.h"
#include "err.h"

#define IAC 255
#define DO 253
#define WILL 251

#define ECHO 1
#define LINEMODE 34

#define LF 10
#define CR 13

const int twenty_seconds = 20;

const std::vector<uint8_t> up{27, 91, 65};
const std::vector<uint8_t> down{27, 91, 66};
const std::vector<uint8_t> init{IAC, WILL, ECHO, IAC, DO, LINEMODE};

const std::vector<uint8_t> clear_screen{27, '[', '2', 'J', 27, '[', 'H'};
std::string header = "------------------------------------------------------------------------\n\r"
                     " SIK Radio\n\r"
                     "------------------------------------------------------------------------\n\r";


std::string footer = "------------------------------------------------------------------------\n\r";
std::string prefix = "    ";

std::string pointer = "  > ";


std::vector<uint8_t> init_seq() {
    return init;
}




std::vector<uint8_t> Menu::draw_menu() {
    std::vector<uint8_t> msg;
    msg.insert(msg.end(), clear_screen.begin(), clear_screen.begin());
    msg.insert(msg.end(), header.begin(), header.end());

    for (auto st = this->stations.begin(); st != this->stations.end(); st++) {
        if (st == this->active) {
            msg.insert(msg.end(), pointer.begin(), pointer.end());
        } else {
            msg.insert(msg.end(), prefix.begin(), prefix.end());
        }
        msg.insert(msg.end(), st->name.begin(), st->name.end());
        msg.push_back('\n');
        msg.push_back('\r');
    }
    msg.insert(msg.end(), footer.begin(), footer.end());
    return msg;
}

int Menu::add_menu(struct sockaddr_in addr, char* name, time_t time) {
    bool found = false;
    if (this->stations.empty()) {
        stations.emplace_back(std::string(name), addr, time);
        if(!this->prefered_set) {
            this->active = stations.begin();
            is_active = true;
        }
        return 1; // changed active
    }
    for (auto& station : this->stations) {
        if (station.address.sin_addr.s_addr == addr.sin_addr.s_addr
            && station.address.sin_port == addr.sin_port) {
            found = true;
            station.time = time;
        }
    }
    if (!found) {
        auto st = this->stations.begin();
        for (; st != this->stations.end(); st++) {
            if (st->name > std::string(name)) {
                break;
            }
        }
        Station temp(std::string(name), addr, time);
        stations.insert(st, temp);
        if(prefered_set && temp.name == prefered_name) {
            active = --st;
            is_active = true;
        }
        return 1; //TODO  redraw menu
    }
    return 0; //TODO do nothing
}

int Menu::transform(uint8_t* buffer) {
    if (this->stations.size() < 2)
        return 0;
    if (memcmp(buffer, &up[0], 3) == 0) {
        if (this->active == this->stations.begin() || this->active == this->stations.end())
            this->active = --this->stations.end();
        else
            this->active--;
        return 1;
    } else if (memcmp(buffer, &down[0], 3) == 0 ) {
        if (this->active == --this->stations.end() || this->active == this->stations.end())
            this->active = this->stations.begin();
        else
            this->active++;
        return 1;
    }
    return 0;
}

Menu::Menu() {
    active = stations.begin();
}

int Menu::check_time(time_t time) {
    bool changed_current = false;
    for (auto station = this->stations.begin(); station != this->stations.end(); station++) {

        if (time - station->time > twenty_seconds) {
            if (station == active) {
                changed_current = true;
            }
            stations.erase(station);
            active = stations.begin();
            if(active == stations.end()) {
                is_active = false;
            }
        }
    }
    return changed_current;
}

Station::Station(const std::string& name, const sockaddr_in& address, time_t time) : name(name), address(address),
                                                                                     time(time) {}
