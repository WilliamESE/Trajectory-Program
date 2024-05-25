#include "expansion.hpp"

using namespace sparkyLib;

Expansion::Expansion(std::shared_ptr<Pneumatic> pistons) 
                : m_pistons(pistons) {}

void Expansion::launch() { 
    m_pistons -> setState(true); 
}