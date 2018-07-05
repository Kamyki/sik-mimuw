//
// Created by Kamil Mykitiuk on 05/06/2018.
//

#include <string.h>
#include <stdlib.h>
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

const uint8_t up[] = {27, 91, 65};
const uint8_t down[] = {27, 91, 66};
const uint8_t init[] = {IAC, WILL, ECHO, IAC, DO, LINEMODE};

const uint8_t clear_screen[] = {27, '[', '2', 'J', 27, '[', 'H'};
const char* header = "------------------------------------------------------------------------\n\r"
                     " SIK Radio\n\r"
                     "------------------------------------------------------------------------\n\r";


const char* footer = "------------------------------------------------------------------------\n\r";
const char* prefix = "    ";

const char* pointer = "  > ";

int transform(Menu* menu, uint8_t* buffer) {
    if (menu->size < 2)
        return 0;
    if (memcmp(buffer, up, 3) == 0) {
        menu->active = menu->active->prev;
        return 1;
    } else if (memcmp(buffer, down, 3) == 0) {
        menu->active = menu->active->next;
        return 1;
    }
    return 0;
}

size_t add_to_draw_buffer(uint8_t* buffer, size_t fill, const void* str) {
    memcpy(buffer + fill, str, fill);
    return fill + strlen(str);
}

size_t draw_menu(Menu* menu, uint8_t* buffer) {
    size_t mem_pointer = 0;
    mem_pointer = add_to_draw_buffer(buffer, mem_pointer, clear_screen);


    mem_pointer = add_to_draw_buffer(buffer, 0, header);
    Station* current = menu->stations;
    for (int i = 0; i < menu->size; i++, current = current->next) {
        if (current == menu->active) {
            mem_pointer = add_to_draw_buffer(buffer, mem_pointer, pointer);
        } else {
            mem_pointer = add_to_draw_buffer(buffer, mem_pointer, prefix);
        }
        mem_pointer = add_to_draw_buffer(buffer, mem_pointer, current->name);
        memcpy(buffer + mem_pointer, "\n\r", 2);
        mem_pointer += 2;
    }
    mem_pointer = add_to_draw_buffer(buffer, mem_pointer, footer);
    return mem_pointer;
}

const uint8_t* init_seq() {
    return init;
}

Menu* create_menu() {
    Menu* menu = malloc(sizeof(Menu));
    menu->stations = NULL;
    menu->active = NULL;
    menu->size = 0;
    return menu;
}

void delete_menu(Menu* menu) {
    if (menu->stations != NULL) {
        free(menu->stations);
    }
    free(menu);
}


int cmp_station(const void* s1, const void* s2) {
    Station a1 = *((Station*) s1);
    Station a2 = *((Station*) s2);
    return strcmp(a1.name, a2.name);
}

//TODO trim name to 64

int add_menu(Menu* menu, struct sockaddr_in addr, char* name, time_t time) {
    int found = 0;
    Station* edit;
    if (menu->size == 0) {
        menu->stations = malloc(sizeof(Station));
        menu->size++;
        edit = menu->stations;
        edit->next = edit;
        edit->prev = edit;
        edit->time = time;
        edit->address = addr;
        strcpy(edit->name, name);
        menu->active = edit;
        return 1; //TODO changed active
    }

    Station* current = menu->stations;
    for (int i = 0; i < menu->size; ++i, current = current->next) {
        if (current->address.sin_addr.s_addr == addr.sin_addr.s_addr
            && menu->stations[i].address.sin_port == addr.sin_port) {
            found = 1;
            current->time = time;
        }
    }
    if (found == 0) {
        menu->size++;
        current = menu->stations;
        for (int i = 0; i < menu->size; ++i, current = current->next) {
            if (strcmp(current->name, name) > 0) {
                break;
            }
        }
        edit = malloc(sizeof(Station));
        edit->prev = current->prev;
        edit->prev->next = edit;
        edit->next = current;
        current->prev = edit;

        edit->address = addr;
        strcpy(edit->name, name);
        edit->socket = -1;
        return 1; //TODO  redraw menu
    }
    return 0; //TODO do nothing
}


int check_time(Menu* menu, time_t time) {
    Station* current = menu->stations;
    int changed_current = 0;
    for (int i = 0; i < menu->size; ++i, current = current->next) {
        if (time - current->time > twenty_seconds) {
            if (menu->size == 1) {
                if (time - current->time > twenty_seconds) {
                    free(current);
                    menu->size = 0;
                    menu->stations = NULL;
                    menu->active = NULL;
                    changed_current = 1;
                }
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                menu->active = current->next;
                menu->size--;
                if (current == menu->active) {
                    changed_current = 1;
                }
            }
        }
    }
    return changed_current;
}
//TODO: Jeśli podano parametr -n, odbiornik rozpoczyna odtwarzanie stacji o zadanej nazwie, gdy tylko ją wykryje

int menu_size(Menu* menu) {
    return menu->size;
}
