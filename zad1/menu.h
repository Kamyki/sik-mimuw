//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#ifndef ZAD1_MENU_H
#define ZAD1_MENU_H

#include <string>
#include <utility>
#include <vector>

class Action;

class Option {

    friend class Menu;

    std::string display_text;

public:
    std::shared_ptr<Action> action;

    Option(std::string display_text, std::shared_ptr<Action> action);

    explicit Option(std::string display_text) : display_text(std::move(display_text)) {}

    void setAction(const std::shared_ptr<Action>& action);

};

class Menu {

    std::shared_ptr<std::vector<Option>> options;
    unsigned active;

public:

    explicit Menu(std::shared_ptr<std::vector<Option>> options) : options(std::move(options)), active(0) {}

    Option& operator[](unsigned long index);

    unsigned getActiveIndex() const;

    void setOptions(std::shared_ptr<std::vector<Option>> options, unsigned active = 0);

    std::vector<std::string> toString() const;

};




#endif //ZAD1_MENU_H
