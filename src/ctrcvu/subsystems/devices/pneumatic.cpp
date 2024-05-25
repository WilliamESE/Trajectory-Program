#include "pneumatic.hpp"

using namespace sparkyLib;

Pneumatic::Pneumatic(pros::ADIDigitalOut ipneumatic, bool initState) : pneumatic(ipneumatic), state(initState) {

}

void Pneumatic::setState(bool istate) {
    pneumatic.set_value(istate);
    state = istate;
}

void Pneumatic::open() {
    setState(true);
}

void Pneumatic::close() {
    setState(false);
}

void Pneumatic::toggle() {
    setState(state);
    state = !state;
}

bool Pneumatic::getState() {
    return state;
}
 