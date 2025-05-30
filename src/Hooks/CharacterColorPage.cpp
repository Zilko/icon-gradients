#include "../Utils/Utils.hpp"
#include "../Utils/Cache.hpp"

#include "CharacterColorPage.hpp"
#include "GJGarageLayer.hpp"

void ProCharacterColorPage::updateGradient() {
    if (Utils::isSettingEnabled(MOD_DISABLED)) return;

    Loader::get()->queueInMainThread([this] {
        CCArrayExt<SimplePlayer*> array = CCArrayExt<SimplePlayer*>(m_playerObjects);

        for (int i = 0; i < array.size(); i++) {
            IconType type =  static_cast<IconType>(i);
            
            if (type == IconType::Ship)
                if (!m_fields->m_isShip)
                    type = IconType::Jetpack;
            
            Utils::applyGradient(array[i], Utils::getSavedConfig(type, false), false, true);
            Utils::applyGradient(array[i], Utils::getSavedConfig(type, true), true, true);
        }
    });
}

bool ProCharacterColorPage::init() {
    if (!CharacterColorPage::init()) return false;

    updateGradient();
    
    return true;
}

void ProCharacterColorPage::toggleShip(CCObject* p0) {
    CharacterColorPage::toggleShip(p0);

    m_fields->m_isShip = !m_fields->m_isShip;

    updateGradient();
}

void ProCharacterColorPage::onPlayerColor(CCObject* sender) {
    CharacterColorPage::onPlayerColor(sender);

    updateGradient();
}

void ProCharacterColorPage::onClose(CCObject* sender) {
    ProGJGarageLayer* garage = static_cast<ProGJGarageLayer*>(getParent());

    CharacterColorPage::onClose(sender);

    garage->updateGradient();
}

void ProCharacterColorPage::keyBackClicked() {
    ProGJGarageLayer* garage = static_cast<ProGJGarageLayer*>(getParent());

    CharacterColorPage::keyBackClicked();

    garage->updateGradient();
}