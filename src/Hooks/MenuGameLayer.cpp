#include "MenuGameLayer.hpp"
#include "PlayerObject.hpp"

#include "../Utils/Utils.hpp"

void ProMenuGameLayer::resetPlayer() {
    MenuGameLayer::resetPlayer();

    if (Utils::isSettingEnabled(MOD_DISABLED) || !Utils::isSettingEnabled(MENU_GRADIENTS) || !m_playerObject) return;

    ProPlayerObject* player = static_cast<ProPlayerObject*>(m_playerObject);

    auto f = player->m_fields.self();

    IconType type = player->getIconType();
    Gradient gradient = Utils::getGradient(type, false);

    if (f->m_thatOneUfoShipAndCubeModIsLoaded) {
        if (f->m_iconSprite) f->m_iconSprite->setVisible(true);
        if (f->m_iconSpriteSecondary) f->m_iconSpriteSecondary->setVisible(true);
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

        player->m_iconSprite->setVisible(true);
        player->m_iconSpriteSecondary->setVisible(true);
        
        player->m_iconSprite->setOpacity(255);
        player->m_iconSpriteSecondary->setOpacity(255);
    }
}