#include "../Utils/Utils.hpp"

#include "PlayLayer.hpp"
#include "PlayerObject.hpp"

void ProPlayLayer::postUpdate(float dt) {
    PlayLayer::postUpdate(dt);

    if (Utils::isSettingEnabled(MOD_DISABLED)) return;

    if (m_player1)
        static_cast<ProPlayerObject*>(m_player1)->updateVisibility();

    if (m_player2)
        static_cast<ProPlayerObject*>(m_player2)->updateVisibility();
}