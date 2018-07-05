//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#include "menu.h"
#include "action.h"
#include <utility>

std::vector<std::string> Menu::toString() const {
    auto texts = std::vector<std::string>();
    for (auto &option: *options) {
        texts.push_back(option.display_text);
    }
    return texts;
}

unsigned Menu::getActiveIndex() const {
    return active;
}

Option& Menu::operator[](unsigned long index) {
    return (*options)[index];
}

void Menu::setOptions(std::shared_ptr<std::vector<Option>> options, unsigned active) {
    this->options = std::move(options);
    this->active = active;
}

Option::Option(std::string display_text, std::shared_ptr<Action> action)
        : display_text(std::move(display_text)), action(std::move(action)) {}

void Option::setAction(const std::shared_ptr<Action>& action) {
    Option::action = action;
}