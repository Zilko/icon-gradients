#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"

#include "PlayLayer.hpp"
#include "PlayerObject.hpp"

void ProPlayLayer::postUpdate(float dt) {
    PlayLayer::postUpdate(dt);

    if (Utils::isSettingEnabled(MOD_DISABLED)) return;

    if (m_player1)
        static_cast<ProPlayerObject*>(m_player1)->updateVisibility();
    static_cast<ProPlayerObject*>(m_player1)->updateFlip(0.f);

    if (m_player2)
        static_cast<ProPlayerObject*>(m_player2)->updateVisibility();
    static_cast<ProPlayerObject*>(m_player2)->updateFlip(0.f);
}