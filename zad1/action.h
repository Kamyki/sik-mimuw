//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#ifndef ZAD1_ACTION_H
#define ZAD1_ACTION_H

#include <string>
#include <utility>

#include "state.h"

class Action {

public:
    virtual State& execute(State& state) = 0;
};

class ClearTerminal: public Action {

public:
    ClearTerminal() = default;

    State& execute(State& state) override;
};


class EnterSubmenu: public Action {

public:

    std::shared_ptr<std::vector<Option>> submenu;
    unsigned active;

    explicit EnterSubmenu() = default;

    explicit EnterSubmenu(std::shared_ptr<std::vector<Option>> submenu, unsigned active = 0)
            : submenu(std::move(submenu)), active(active) {}

    State& execute(State& state) override;
};

class AddText: public Action {

    std::string text;

public:
    explicit AddText(std::string text) : text(std::move(text)) {}

    State& execute(State& state) override;

};




#endif //ZAD1_ACTION_H
