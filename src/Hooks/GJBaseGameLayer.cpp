#include "GJBaseGameLayer.hpp"
#include "SimplePlayer.hpp"

#include "Utils/Utils.hpp"

void ProGJBaseGameLayer::playExitDualEffect(PlayerObject* p0) {
    if (!p0->isVanillaPlayer() || Utils::isSettingEnabled(MOD_DISABLED)) {
        GJBaseGameLayer::playExitDualEffect(p0);
        return;
    }

    auto f = m_fields.self();
    
    f->isExitingDual = true;

    GJBaseGameLayer::playExitDualEffect(p0);

    if (auto icon = static_cast<ProSimplePlayer*>(f->dualSimplePlayer)) {
        Utils::applyGradient(
            icon,
            Utils::getGradient(icon->m_fields->m_type, p0 == m_player2);,
            true,
            p0 == m_player2,
            1000
        );
    }

    f->isExitingDual = false;
    f->dualSimplePlayer = nullptr;
}