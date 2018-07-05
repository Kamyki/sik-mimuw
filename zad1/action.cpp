#include "action.h"



State& ClearTerminal::execute(State& state) {
    for (unsigned long i = 0; i < state.getLinesSize(); ++i) {
        state.clearLine(i);
    }
    return state;
}

State& EnterSubmenu::execute(State& state) {
    state.setActive_menu(submenu);
    return state;
}


State& AddText::execute(State& state) {
    state.setTextLine(0, text);
    return state;
}
