//
// Created by Kamil Mykitiuk on 07/05/2018.
//

#include "state.h"
#include "menu.h"
#include <string>

const std::vector<uint8_t> clear_screen_seq() {
    static std::vector<uint8_t> a{ESCAPE, '[', 'J'};
    return a;
}

const std::vector<uint8_t> hide_cursor_seq() {
    static std::vector<uint8_t> a{ESCAPE, '[', '?', '2', '5', 'l'};
    return a;
}

const std::vector<uint8_t> show_cursow_seq() {
    static std::vector<uint8_t> a{ESCAPE, '[', '?', '2', '5', 'h'};
    return a;
}

const std::vector<uint8_t> clear_line_seq() {
    static std::vector<uint8_t> a{ESCAPE, '[', '2', 'K'};
    return a;
}

const std::vector<uint8_t> normal_style_seq() {
    static std::vector<uint8_t> a{ESCAPE, '[', '0', 'm'};
    return a;
}

const std::vector<uint8_t> reverse_style_seq() {
    static std::vector<uint8_t> a {ESCAPE, '[', '7', 'm'};
    return a;
}

unsigned long State::getLinesSize() const {
    return lines.size();
}

void State::clearLine(unsigned long i) {
    lines[i].clear();
    lines[i] = clear_line_seq();
}

void State::setTextLine(unsigned long i, const std::string& text) {
    lines[i] = line(text.begin(), text.end());

}

std::vector<uint8_t> State::toMessage() {
    auto seq = std::vector<uint8_t>();

    int i = 0;
    for (auto option: active_menu->toString()) {
        if (i == active_menu->getActiveIndex()) {
            seq.insert(seq.end(), reverse_style_seq().begin(), reverse_style_seq().end());
        }

        seq.insert(seq.end(), option.begin(), option.end());

        if (i == active_menu->getActiveIndex()) {
            seq.insert(seq.end(), normal_style_seq().begin(), normal_style_seq().end());
        }
        seq.push_back(CR);
        seq.push_back(LF);
    }

    for (auto line: lines) {
        seq.insert(seq.end(), line.begin(), line.end());
        seq.push_back(CR);
        seq.push_back(LF);
    }
    return seq;
}

void State::setActive_menu(std::shared_ptr<std::vector<Option>> options, unsigned active) {
    this->active_menu->setOptions(options, active);
}

State::State(const std::shared_ptr<Menu>& active_menu, unsigned lines_count) : active_menu(
        active_menu), lines(lines_count) {}

std::vector<uint8_t> State::initial() {
    std::vector<uint8_t> seq;
    seq.insert(seq.end(), hide_cursor_seq().begin(), hide_cursor_seq().end());
    seq.push_back(CR);
    seq.push_back(LF);
    return seq;
}

Option State::getOption() {
    auto i = active_menu->getActiveIndex();
    return (*active_menu)[i];
}

