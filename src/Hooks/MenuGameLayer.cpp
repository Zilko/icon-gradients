#include "MenuGameLayer.hpp"
#include "PlayerObject.hpp"

#include "../Utils/Utils.hpp"

bool ProMenuGameLayer::init() {
    if (!MenuGameLayer::init()) return false;

    m_fields->m_realPlayerObject = m_playerObject;

    return true;
}

void ProMenuGameLayer::resetPlayer() {
    MenuGameLayer::resetPlayer();

    if (
        Utils::isSettingEnabled(MOD_DISABLED)
        || !Utils::isSettingEnabled(MENU_GRADIENTS)
        || !m_playerObject
        || Loader::get()->isModLoaded("iandyhd3.known_players")
    ) {
        return;
    }

    if (!m_fields->m_realPlayerObject) return;

    ProPlayerObject* player = static_cast<ProPlayerObject*>(m_fields->m_realPlayerObject);

    auto f = player->m_fields.self();

    IconType type = player->getIconType();
    Gradient gradient = Utils::getGradient(type, false);

    if (f->m_thatOneUfoShipAndCubeModIsLoaded) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(true);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(true);
        if (f->m_iconGlow) f->m_iconGlow->setVisible(true);
    }

    if (type != IconType::Ship && type != IconType::Jetpack && type != IconType::Ufo)
        return player->updateIconSprite(gradient, f);

    player->updateVehicleSprite(gradient, f);

    player->updateIconSprite(
        Utils::getGradient(IconType::Cube, false),
        f
    );

    if (f->m_thatOneUfoShipAndCubeModIsLoaded) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(false);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(false);
        if (f->m_iconGlow) f->m_iconGlow->setVisible(false);

        player->m_iconSprite->setVisible(true);
        player->m_iconSpriteSecondary->setVisible(true);
        
        player->m_iconSprite->setOpacity(255);
        player->m_iconSpriteSecondary->setOpacity(255);
    }
}