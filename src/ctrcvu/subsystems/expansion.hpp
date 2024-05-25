#pragma once

#include "devices/pneumatic.hpp" 

namespace sparkyLib {
    class Expansion {
        public:
        std::shared_ptr<Pneumatic> m_pistons;

        Expansion(std::shared_ptr<Pneumatic> pistons);
        
        /**
        * @brief pew
        *
        */
        void launch();
    };
} // namespace sparkyLib