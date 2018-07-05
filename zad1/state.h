//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#ifndef ZAD1_STATE_H
#define ZAD1_STATE_H

#include <memory>
#include <vector>

class Menu;
class Option;

const uint8_t ESCAPE = 27;
const uint8_t CR = 13;
const uint8_t LF = 10;

class State {

    using line = std::vector<uint8_t>;

    std::shared_ptr<Menu> active_menu;
    std::vector<line> lines;

public:
    State(const std::shared_ptr<Menu>& active_menu, unsigned lines_count);

    unsigned long getLinesSize() const;

    void clearLine(unsigned long i);

    void setTextLine(unsigned long i, const std::string &text);

    void setActive_menu(std::shared_ptr<std::vector<Option>> options, unsigned active = 0);

    std::vector<uint8_t> toMessage();

    std::vector<uint8_t> initial();

    Option getOption();
};


#endif //ZAD1_STATE_H
