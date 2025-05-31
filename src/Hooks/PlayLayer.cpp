#include "PlayLayer.hpp"
#include "PlayerObject.hpp"

void ProPlayLayer::resetLevel() {
    PlayLayer::resetLevel();

    if (m_player1)
        static_cast<ProPlayerObject*>(m_player1)->updateVisibility();

    if (m_player2)
        static_cast<ProPlayerObject*>(m_player2)->updateVisibility();
}